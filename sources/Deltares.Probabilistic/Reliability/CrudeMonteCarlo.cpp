// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "CrudeMonteCarlo.h"
#include <vector>
#include <cmath>

#include "../Statistics/StandardNormal.h"
#include "../Model/Sample.h"
#include "../Model/RandomSampleGenerator.h"
#include "../Math/NumericSupport.h"
#include "ConvergenceReport.h"
#include "ReliabilityReport.h"
#include "DesignPoint.h"
#include "DesignPointBuilder.h"
#include "StochastSettings.h"

using namespace Deltares::Models;

namespace Deltares::Reliability
{
    std::shared_ptr<DesignPoint> CrudeMonteCarlo::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
    {
        modelRunner->updateStochastSettings(this->Settings->StochastSet);

        std::shared_ptr<SampleProvider> sampleProvider = std::make_shared<SampleProvider>(*Settings->StochastSet);
        modelRunner->setSampleProvider(sampleProvider);

        double qRange = 1;
        double zRemainder = 1;

        Sample remainderSample = sampleProvider->getSample();

        for (int i = 0; i < this->Settings->StochastSet->getVaryingStochastCount(); i++)
        {
            if (!this->Settings->StochastSet->VaryingStochastSettings[i]->isMinMaxDefault())
            {
                double probLow = Statistics::StandardNormal::getPFromU(this->Settings->StochastSet->VaryingStochastSettings[i]->MinValue);
                double probHigh = Statistics::StandardNormal::getQFromU(this->Settings->StochastSet->VaryingStochastSettings[i]->MaxValue);

                double prob = 1 - probLow - probHigh;

                qRange *= prob;

                if (probLow > probHigh)
                {
                    remainderSample.Values[i] = this->Settings->StochastSet->VaryingStochastSettings[i]->MinValue - 0.1;
                }
                else
                {
                    remainderSample.Values[i] = this->Settings->StochastSet->VaryingStochastSettings[i]->MaxValue + 0.1;
                }
            }
        }

        if (qRange < 1)
        {
            // perform one run to identify whether the remainder is failing
            zRemainder = modelRunner->getZValue(remainderSample);
        }

        return getReducedDesignPoint(modelRunner, sampleProvider, zRemainder, qRange);
    }

    std::shared_ptr<DesignPoint> CrudeMonteCarlo::getReducedDesignPoint(const std::shared_ptr<ModelRunner>&modelRunner,
        const std::shared_ptr<SampleProvider>& sampleProvider, double zRemainder, double qRange)
    {
        auto randomSampleGenerator = RandomSampleGenerator();
        randomSampleGenerator.Settings = this->Settings->randomSettings;
        randomSampleGenerator.Settings->StochastSet = this->Settings->StochastSet;
        randomSampleGenerator.sampleProvider = sampleProvider;
        randomSampleGenerator.initialize();

        auto statistics = Numeric::StatisticsCalculator();

        int nParameters = modelRunner->getVaryingStochastCount();
        std::vector<double> zValues; // copy of z for all parallel threads as double

        auto designPointBuilder = DesignPointBuilder(nParameters, Settings->designPointMethod, this->Settings->StochastSet, Settings->RunSettings->shouldAddProbability());

        bool initial = true;
        double z0Fac = 0.0;
        double nFailed = 0.0;
        int nSamples = 0;
        const std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();
        std::vector<Sample> samples;
        size_t zIndex = 0;

        bool addRemainder = qRange != 1.0;
        double qRangeAdditional = addRemainder ? (1.0 - qRange) / qRange : 0.0;
        double zRemainderAddition = addRemainder ? getFailureAddition(zRemainder, Settings->RunSettings->modelReturnType) : std::nan("");

        for (int sampleIndex = 0; sampleIndex < Settings->MaximumSamples + 1 && !isStopped(); sampleIndex++)
        {
            zIndex++;

            if (initial || zIndex >= samples.size())
            {
                samples.clear();

                int chunkSize = modelRunner->Settings->MaxChunkSize;
                int runs = std::min(chunkSize, Settings->MaximumSamples + 1 - sampleIndex);

                if (initial)
                {
                    samples.push_back(sampleProvider->getSample());
                    runs = runs - 1;
                }

                for (int i = 0; i < runs; i++)
                {
                    Sample sample = randomSampleGenerator.getRandomSample();
                    if (qRange < 1)
                    {
                        applyLimits(sample);
                    }

                    samples.push_back(sample);
                }

                zValues = modelRunner->getZValues(samples);

                if (initial)
                {
                    z0Fac = getZFactor(zValues[0], Settings->RunSettings->modelReturnType);
                    designPointBuilder.initialize(z0Fac * Statistics::StandardNormal::BetaMax);
                }

                if (modelRunner->shouldExitPrematurely(samples))
                {
                    // return the result so far
                    auto uMin = designPointBuilder.getSample();
                    double pf = statistics.getMean();

                    if (std::isnan(pf))
                    {
                        pf = 0.0;
                    }

                    return modelRunner->getDesignPoint(uMin, Statistics::StandardNormal::getUFromQ(pf), convergenceReport);
                }

                zIndex = 0;
            }

            if (initial)
            {
                z0Fac = getZFactor(zValues[zIndex], Settings->RunSettings->modelReturnType);
                initial = false;
                continue;
            }

            double z = zValues[zIndex];
            Sample u = samples[zIndex];

            if (std::isnan(z))
            {
                continue;
            }

            nSamples++;

            double failureAddition = getFailureAddition(z, Settings->RunSettings->modelReturnType);
            nFailed += failureAddition;

            statistics.addValue(failureAddition);

            // for the remainder, add artificial sample for each sample so that overall mean and convergence are correct
            if (addRemainder)
            {
                statistics.addValue(zRemainderAddition, qRangeAdditional);
            }

            if (failureAddition > 0.0)
            {
                convergenceReport->FailedSamples += 1;
            }
            convergenceReport->FailFraction = nFailed / nSamples;

            double smallestDomainAddition = z0Fac > 0.0 ? failureAddition : 1.0 - failureAddition;

            if (smallestDomainAddition > 0)
            {
                designPointBuilder.addSample(u, smallestDomainAddition);
            }

            convergenceReport->IsConverged = checkConvergence(modelRunner, statistics, sampleIndex);

            if (convergenceReport->IsConverged)
            {
                break;
            }
        }

        double pf = statistics.getMean();
        double beta = Statistics::StandardNormal::getUFromQ(pf);
        auto uMin = designPointBuilder.getSample();

        convergenceReport->Convergence = getConvergence(statistics);

        std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(uMin, beta, convergenceReport);

        samples.clear();

        return designPoint;
    }

    void CrudeMonteCarlo::applyLimits(Sample& sample) const
    {
        for (int i = 0; i < sample.getSize(); i++)
        {
            std::shared_ptr<StochastSettings> settings = this->Settings->StochastSet->VaryingStochastSettings[i];
            if (!settings->isMinMaxDefault())
            {
                double q = Statistics::StandardNormal::getPFromU(sample.Values[i]);
                q = settings->XMinValue + q * (settings->XMaxValue - settings->XMinValue);
                sample.Values[i] = Statistics::StandardNormal::getUFromP(q);
            }
        }
    }

    bool CrudeMonteCarlo::checkConvergence(const std::shared_ptr<ModelRunner>& modelRunner,
        const Numeric::StatisticsCalculator& statistics, int nmaal) const
    {
        std::shared_ptr<ReliabilityReport> report(new ReliabilityReport());
        report->Step = nmaal;
        report->MaxSteps = Settings->MaximumSamples;

        double pf = statistics.getMean();

        if (pf > 0 && pf < 1)
        {
            double convergence = getConvergence(statistics);
            report->Reliability = Statistics::StandardNormal::getUFromQ(pf);
            report->Variation = convergence;
            modelRunner->reportResult(report);
            bool enoughSamples = nmaal >= Settings->MinimumSamples;
            return enoughSamples && convergence < Settings->VariationCoefficient;
        }
        else
        {
            modelRunner->reportResult(report);
            return false;
        }
    }

    double CrudeMonteCarlo::getConvergence(const Numeric::StatisticsCalculator& statistics)
    {
        double pf = statistics.getMean();

        if (pf > 0 && pf < 1)
        {
            if (pf > 0.5)
            {
                pf = 1 - pf;
            }

            // standard error
            double standardError = statistics.getStandardDeviation() / sqrt(statistics.getCount());

            // variation coefficient
            double variationCoefficient = standardError / pf;

            return variationCoefficient;
        }
        else
        {
            return nan("");
        }
    }
}

