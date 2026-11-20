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
#include <algorithm>
#include "LatinHyperCube.h"
#include "DesignPointBuilder.h"
#include "../Statistics/StandardNormal.h"
#include "../Math/NumericSupport.h"
#include "../Math/RandomValueGenerator.h"
#include "../Math/ShuffleItems.h"

using namespace Deltares::Models;
using namespace Deltares::Statistics;
using namespace Deltares::Numeric;

namespace Deltares::Reliability
{
    std::shared_ptr<DesignPoint> LatinHyperCube::getDesignPoint(std::shared_ptr<ModelRunner> modelRunner)
    {
        modelRunner->updateStochastSettings(this->Settings->StochastSet);

        auto sampleProvider = std::make_shared<SampleProvider>(*Settings->StochastSet);
        modelRunner->setSampleProvider(sampleProvider);

        double qRange = 1.0;

        Sample remainderSample = sampleProvider->getSample();

        for (int i = 0; i < this->Settings->StochastSet->getVaryingStochastCount(); i++)
        {
            if (!this->Settings->StochastSet->VaryingStochastSettings[i]->isMinMaxDefault())
            {
                double probLow = StandardNormal::getPFromU(this->Settings->StochastSet->VaryingStochastSettings[i]->MinValue);
                double probHigh = StandardNormal::getQFromU(this->Settings->StochastSet->VaryingStochastSettings[i]->MaxValue);

                double prob = 1.0 - probLow - probHigh;

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

        double remainder_failure = 0.0;
        if (qRange < 1.0)
        {
            const double zRemainder = modelRunner->getZValue(remainder_sample);
            remainder_failure = std::isnan(zRemainder)
                ? zRemainder
                : getFailureAddition(zRemainder, Settings->RunSettings->modelReturnType);
        }

        return getReducedDesignPoint(*modelRunner, qRange, remainder_failure);
    }

    std::vector<Sample*> LatinHyperCube::createAllSamples(int nStochasts, SampleStorage& storage) const
    {
        RandomValueGenerator random;
        random.initialize(Settings->randomSettings->IsRepeatableRandom, Settings->randomSettings->Seed,
                          Settings->randomSettings->getTimeStamp());

        // Create all indices
        std::vector<std::vector<int>> indices;
        for (int i = 0; i < nStochasts; i++)
        {
            std::vector<int> iValues;
            iValues.reserve(Settings->MinimumSamples);
            for (int j = 0; j < Settings->MinimumSamples; j++)
            {
                iValues.push_back(j);
            }
            ShuffleItems<int>::ShuffleInPlace(iValues, random);
            indices.push_back(iValues);
        }

        // Create all samples
        std::vector<double> pMinValues;
        std::vector<double> pMaxValues;
        for (int i = 0; i < nStochasts; i++)
        {
            double uMin = Settings->StochastSet->VaryingStochastSettings[i]->MinValue;
            double uMax = Settings->StochastSet->VaryingStochastSettings[i]->MaxValue;
            pMinValues.push_back(StandardNormal::getQFromU(uMin));
            pMaxValues.push_back(StandardNormal::getQFromU(uMax));
        }

        std::vector<Sample*> samples;
        for (int n = 0; n < Settings->MinimumSamples; n++)
        {
            std::vector<int> uIndices;
            uIndices.reserve(nStochasts);
            for (int i = 0; i < nStochasts; i++)
            {
                uIndices.push_back(indices[i][n]);
            }

            std::vector<double> uValues(nStochasts);

            for (int i = 0; i < nStochasts; i++)
            {
                double pMin = pMinValues[i];
                double pMax = pMaxValues[i];
                double step = (pMin - pMax) / Settings->MinimumSamples;

                uValues[i] = StandardNormal::getUFromQ(pMin - (uIndices[i] + 0.5) * step);
            }

            auto sample = Sample(uValues);
            sample.Weight = 1.0 / Settings->MinimumSamples;
            samples.push_back(storage.keep(sample));
        }
        return samples;
    }

    std::shared_ptr<DesignPoint> LatinHyperCube::getReducedDesignPoint(ModelRunner& modelRunner, double qRange, double remainder_failure)
    {
        const int nStochasts = modelRunner.getVaryingStochastCount();

        auto zValues = std::vector<double>(0); // copy of z for all parallel threads as double

        auto uMean = DesignPointBuilder(nStochasts, Settings->designPointMethod, this->Settings->StochastSet);

        Sample uMin = Sample(nStochasts);

        // initialise convergence indicator and loops
        bool initial = true;
        double z0Fac = 0.0;

        int nFailed = 0;
        int nSamples = 0;
        auto convergence_report = std::make_shared<ConvergenceReport>();
        const int chunk_size = Settings->RunSettings->MaxChunkSize;

        size_t zIndex = 0;

        SampleStorage storage = SampleStorage(Settings->MinimumSamples + 1);

        auto samples = createAllSamples(nStochasts, storage);

        std::vector<Sample*> calc_samples;
        double prob_failure = (1.0 - qRange) * remainder_failure;

        for (int n = 0; n < Settings->MinimumSamples && !isStopped(); n++)
        {
            zIndex++;

            if (initial || zIndex >= zValues.size())
            {
                calc_samples.clear();

                int runs = std::min(chunk_size, Settings->MinimumSamples - n);

                if (initial)
                {
                    Sample sample = Sample(nStochasts);
                    calc_samples.push_back(storage.keep(sample));
                    runs = std::max(1, runs - 1);
                }

                for (int i = 0; i < runs; i++)
                {
                    calc_samples.push_back(samples[n + i]);
                }

                zValues = modelRunner.getZValues(calc_samples);

                if (initial)
                {
                    // determine multiplication factor for z (z<0), if u = 0
                    z0Fac = getZFactor(zValues[0]);
                    uMin = Sample(uMin.getSampleAtBeta(z0Fac * StandardNormal::BetaMax));
                    uMean.initialize(z0Fac * StandardNormal::BetaMax);
                    initial = false;
                    zIndex = 1;
                }
                else
                {
                    zIndex = 0;
                }
            }


            double z = zValues[zIndex];
            Sample u = *calc_samples[zIndex];

            // ignore a failed evaluation
            if (std::isnan(z))
            {
                continue;
            }

            // increase the number of succeeded samples
            nSamples++;

            // determine total probability of failure
            if (z < 0) nFailed++;

            convergence_report->FailedSamples = nFailed;
            convergence_report->FailFraction = NumericSupport::Divide(nFailed, nSamples);

            // register minimum value of r and alpha
            if (z * z0Fac < 0)
            {
                uMean.addSample(u);
            }

            prob_failure = qRange * convergence_report->FailFraction + (1.0 - qRange) * remainder_failure;

            // add convergence report to model runner
            convergence_report->Convergence = ReportConvergence(modelRunner, prob_failure, nSamples, n);
        }

        // general processing
        double beta = StandardNormal::getUFromQ(prob_failure);

        uMin = uMean.getSample();

        auto design_point = modelRunner.getDesignPoint(uMin, beta, convergence_report);
        return design_point;
    }

    double LatinHyperCube::ReportConvergence(ModelRunner& modelRunner, double pf, int samples, int index) const
    {
        auto report = std::make_shared<ReliabilityReport>(index);

        if (pf > 0.0 && pf < 1.0)
        {
            double varPf = std::sqrt((1.0 - pf) / (samples * pf));
            double varNp = std::sqrt(pf / (samples * (1.0 - pf)));

            double convergence = std::max(varPf, varNp);

            report->Reliability = StandardNormal::getUFromQ(pf);
            report->Variation = convergence;
            report->Step = samples;
            report->MaxSteps = Settings->MinimumSamples;
            report->ConvBeta = convergence;

            modelRunner.reportResult(report);

            return convergence;
        }
        else
        {
            modelRunner.reportResult(report);
            return std::nan("");
        }
    }

}
