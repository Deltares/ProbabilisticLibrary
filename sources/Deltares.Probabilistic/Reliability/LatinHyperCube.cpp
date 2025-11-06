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

using namespace Deltares::Models;
using namespace Deltares::Statistics;
using namespace Deltares::Numeric;

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> LatinHyperCube::getDesignPoint(std::shared_ptr<ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            auto sampleProvider = std::make_shared<SampleProvider>(this->Settings->StochastSet);
            modelRunner->setSampleProvider(sampleProvider);

            double qRange = 1.0;

            std::shared_ptr<Sample> remainderSample = sampleProvider->getSample();

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
                        remainderSample->Values[i] = this->Settings->StochastSet->VaryingStochastSettings[i]->MinValue - 0.1;
                    }
                    else
                    {
                        remainderSample->Values[i] = this->Settings->StochastSet->VaryingStochastSettings[i]->MaxValue + 0.1;
                    }
                }
            }

            return getReducedDesignPoint(modelRunner, qRange);
        };

        std::vector<std::shared_ptr<Sample>> LatinHyperCube::CreateAllSamples(int nStochasts)
        {
            struct IndexedItem
            {
                int Value = 0;
                double Sequence = 0.0;
            };

            RandomValueGenerator random;
            random.initialize(Settings->randomSettings->IsRepeatableRandom, Settings->randomSettings->Seed,
                              Settings->randomSettings->getTimeStamp());

            std::vector<std::pair<int, std::vector<double>>> list;
            for (int i = 0; i < Settings->MinimumSamples; i++)
            {
                auto values = std::vector<double>();
                for (int j = 0; j < nStochasts; j++)
                {
                    auto s = random.next();
                    values.push_back(s);
                }
                list.push_back({ i, values });
            }

            // Create all indices
            std::vector<std::vector<int>> indices;
            for (int i = 0; i < nStochasts; i++)
            {
                std::vector<IndexedItem> values;
                for (const auto& p : list)
                {
                    auto temp_var = IndexedItem();
                    temp_var.Value = p.first;
                    temp_var.Sequence = p.second[i];
                    values.push_back(temp_var);
                }

                std::sort(values.begin(), values.end(), [](const auto& lhs, const auto& rhs)
                    {
                        return lhs.Sequence < rhs.Sequence;
                    });

                std::vector<int> iValues;
                for (const auto& p : values)
                {
                    iValues.push_back(p.Value);
                }
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

            std::vector<std::shared_ptr<Sample>> samples;
            for (int n = 0; n < Settings->MinimumSamples; n++)
            {
                std::vector<int> uIndices;
                for (int i = 0; i < nStochasts; i++)
                {
                    uIndices.push_back(indices[i][n]);
                }

                std::vector<double> uValues(nStochasts);
                double weight = 0.0;

                for (int i = 0; i < nStochasts; i++)
                {
                    double pMin = pMinValues[i];
                    double pMax = pMaxValues[i];
                    double step = (pMin - pMax) / Settings->MinimumSamples;

                    double pLow = pMin - uIndices[i] * step;
                    double pHigh = pMin - (uIndices[i] + 1) * step;
                    uValues[i] = StandardNormal::getUFromQ(pMin - (uIndices[i] + 0.5) * step);
                    weight += pLow - pHigh;
                }

                auto sample = std::make_shared<Sample>(uValues);
                sample->Weight = weight / nStochasts;
                samples.push_back(sample);
            }
            return samples;
        }

        std::shared_ptr<DesignPoint> LatinHyperCube::getReducedDesignPoint(std::shared_ptr<ModelRunner>& modelRunner, double qRange)
        {
            int nStochasts = modelRunner->getVaryingStochastCount();

            auto zValues = std::vector<double>(0); // copy of z for all parallel threads as double

            const std::shared_ptr<DesignPointBuilder> uMean = std::make_shared<DesignPointBuilder>(nStochasts, Settings->designPointMethod, this->Settings->StochastSet);

            std::shared_ptr<Sample> uMin = std::make_shared<Sample>(nStochasts);

            // initialise convergence indicator and loops
            bool initial = true;
            double z0Fac = 0.0;

            int nFailed = 0;
            int nSamples = 0;
            std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

            size_t zIndex = 0;

            auto samples = CreateAllSamples(nStochasts);

            std::vector<std::shared_ptr<Sample>> calcSamples;
            double probFailure = 0.0;
            const int chunkSize = Settings->RunSettings->MaxChunkSize;

            for (int n = 0; n < Settings->MinimumSamples && !isStopped(); n++)
            {
                zIndex++;

                if (initial || zIndex >= zValues.size())
                {
                    calcSamples.clear();

                    int runs = std::min(chunkSize, Settings->MinimumSamples - n);

                    if (initial)
                    {
                        calcSamples.push_back(std::make_shared<Sample>(nStochasts));
                        runs = std::max(1, runs - 1);
                    }

                    for (int i = 0; i < runs; i++)
                    {
                        calcSamples.push_back(samples[n + i]);
                    }

                    zValues = modelRunner->getZValues(calcSamples);

                    if (initial)
                    {
                        // determine multiplication factor for z (z<0), if u = 0
                        z0Fac = getZFactor(zValues[0]);
                        uMin = uMin->getSampleAtBeta(z0Fac * StandardNormal::BetaMax);
                        uMean->initialize(z0Fac * StandardNormal::BetaMax);
                        initial = false;
                        zIndex = 1;
                    }
                    else
                    {
                        zIndex = 0;
                    }
                }


                double z = zValues[zIndex];
                std::shared_ptr<Sample> u = calcSamples[zIndex];

                // ignore a failed evaluation
                if (std::isnan(z))
                {
                    continue;
                }

                // increase the number of succeeded samples
                nSamples++;

                // determine total probability of failure
                if (z < 0) nFailed++;

                convergenceReport->FailedSamples = nFailed;
                convergenceReport->FailFraction = NumericSupport::Divide(nFailed, nSamples);

                // register minimum value of r and alpha
                if (z * z0Fac < 0)
                {
                    uMean->addSample(u);
                }

                if (z < 0.0)
                {
                    probFailure += qRange * u->Weight;
                }

                // check if convergence is reached (or stop criterion)
                convergenceReport->Convergence = ReportConvergence(modelRunner, probFailure, nSamples, n);
            }

            // general processing
            double beta = StandardNormal::getUFromQ(probFailure);

            uMin = uMean->getSample();

            std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(uMin, beta, convergenceReport);
            return designPoint;
        }

        double LatinHyperCube::ReportConvergence(std::shared_ptr<ModelRunner>& modelRunner, double pf, int samples, int Index) const
        {
            auto report = std::make_shared<ReliabilityReport>(Index);

            if (pf > 0.0 && pf < 1.0)
            {
                double varPf = std::sqrt((1.0 - pf) / (samples * pf));
                double varNp = std::sqrt(pf / (samples * (1.0 - pf)));

                double convergence = std::max(varPf, varNp);

                report->Reliability = StandardNormal::getUFromQ(pf);
                report->Variation = convergence;

                modelRunner->reportResult(report);

                return convergence;
            }
            else
            {
                modelRunner->reportResult(report);
                return std::nan("");
            }
        }

    }
}
