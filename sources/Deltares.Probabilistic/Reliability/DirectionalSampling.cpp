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
#include "DirectionalSampling.h"
#include "DirectionReliabilityDS.h"
#include "PrecomputeDirections.h"
#include "../Model/RandomSampleGenerator.h"
#include "../Math/SpecialFunctions.h"
#include <omp.h>
#include <algorithm>

namespace Deltares
{
    namespace Reliability
    {
        using namespace Statistics;
        using namespace Numeric;

        std::shared_ptr<DesignPoint> DirectionalSampling::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            int nStochasts = modelRunner->getVaryingStochastCount();
            double pf = 0;
            double qtot = 0;
            double minBetaDirection = 200; // initialize convergence indicator and loops
            auto designPointBuilder = DesignPointBuilder (nStochasts, Settings->designPointMethod, Settings->StochastSet);
            int parSamples = 0;

            std::vector<double> betaValues;
            std::vector<std::shared_ptr<Models::Sample>> samples;

            modelRunner->updateStochastSettings(Settings->StochastSet);

            std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

            auto randomSampleGenerator = Models::RandomSampleGenerator();
            randomSampleGenerator.Settings = Settings->randomSettings;
            randomSampleGenerator.Settings->StochastSet = Settings->StochastSet;
            randomSampleGenerator.initialize();

            std::shared_ptr<Models::Sample> zeroSample = std::make_shared<Models::Sample>(nStochasts);
            double z0 = modelRunner->getZValue(zeroSample);
            double z0Fac = getZFactor(z0);

            double sumWeights = 0.0;
            double sumWeights2 = 0.0;

            int validSamples = 0;

            if (modelRunner->Settings->MaxParallelProcesses > 0)
            {
                if (Settings->runSettings->UseOpenMPinReliability)
                {
                    omp_set_num_threads(modelRunner->Settings->MaxParallelProcesses);
                }
                else
                {
                    omp_set_num_threads(1);
                }
            }

            int chunkSize = modelRunner->Settings->MaxChunkSize;
            if (modelRunner->ProxySettings->IsProxyModel)
            {
                // the early return for proxy models has results depending on the chunk size
                // to avoid this, use a chunk size equal to the number of processes that are used.
                chunkSize = std::min(chunkSize, modelRunner->Settings->MaxParallelProcesses);
            }

            // loop for all directions
            for (int directionIndex = 0; directionIndex < Settings->MaximumDirections && !isStopped(); directionIndex++)
            {
                if (directionIndex % chunkSize == 0)
                {
                    // check whether restart is needed
                    if (modelRunner->shouldExitPrematurely(samples))
                    {
                        // return the result so far
                        return modelRunner->getDesignPoint(designPointBuilder.getSample(),
                            StandardNormal::getUFromQ(pf), convergenceReport);
                    }

                    samples.clear();

                    int runs = std::min(chunkSize, Settings->MaximumDirections - parSamples * chunkSize);

                    // run max par samples times zrfunc in parallel
                    for (int i = 0; i < runs; i++)
                    {
                        auto sample = randomSampleGenerator.getRandomSample();
                        sample->IterationIndex = directionIndex + i;
                        samples.push_back(sample);
                    }

                    betaValues = getDirectionBetas(*modelRunner, samples, z0, minBetaDirection);

                    parSamples++;
                }

                if (betaValues[directionIndex % chunkSize] * z0 < 0 || std::isnan(betaValues[directionIndex % chunkSize]))
                {
                    continue;
                }

                std::shared_ptr<Models::Sample> u = samples[directionIndex % chunkSize];

                double betaDirection = std::abs(betaValues[directionIndex % chunkSize]);

                // get the sample at the limit state
                std::shared_ptr<Models::Sample> uSurface = u->getSampleAtBeta(betaDirection);

                // calculate failure probability
                if (betaDirection >= 0 && betaDirection < StandardNormal::BetaMax * nStochasts)
                {
                    uSurface->Weight = SpecialFunctions::getGammaUpperRegularized(0.5 * nStochasts, 0.5 * betaDirection * betaDirection);

                    qtot += uSurface->Weight;

                    designPointBuilder.addSample(uSurface);
                }
                else
                {
                    uSurface->Weight = 0;
                }

                validSamples++;

                pf = qtot / static_cast<double>(validSamples);

                // store minimum value of r and alpha
                minBetaDirection = std::min(betaDirection, minBetaDirection);

                // check on convergence criterion
                bool enoughSamples = directionIndex >= Settings->MinimumDirections;
                convergenceReport->TotalDirections = directionIndex+1;

                sumWeights += uSurface->Weight;
                sumWeights2 += uSurface->Weight * uSurface->Weight;

                if (qtot > 0)
                {
                    // determine accuracy

                    double convergence = getConvergence(pf, sumWeights, sumWeights2, validSamples);

                    double beta = z0Fac * StandardNormal::getUFromQ(pf);

                    std::shared_ptr<ReliabilityReport> report = std::make_shared<ReliabilityReport>();
                    report->ReportMatchesEvaluation = false;
                    report->Contribution = uSurface->Weight;
                    report->Reliability = beta;
                    report->Variation = convergence;

                    modelRunner->reportResult(report);

                    if (enoughSamples && convergence <= Settings->VariationCoefficient)
                    {
                        break;
                    }
                }
                else
                {
                    std::shared_ptr<ReliabilityReport> report = std::make_shared<ReliabilityReport>();
                    report->ReportMatchesEvaluation = false;
                    report->Variation = uSurface->Weight;

                    modelRunner->reportResult(report);
                }
            }

            convergenceReport->Convergence = getConvergence(pf, sumWeights, sumWeights2, validSamples);
            convergenceReport->IsConverged = (convergenceReport->Convergence <= Settings->VariationCoefficient);

            auto uDesign = designPointBuilder.getSample();

            if (z0 < 0)
            {
                pf = 1 - pf;
            }

            samples.clear();

            return modelRunner->getDesignPoint(uDesign, StandardNormal::getUFromQ(pf), convergenceReport, "Directional Sampling");
        }

        double DirectionalSampling::getConvergence(const double pf, const double sumWeights, const double sumWeights2, const int nDirections)
        {
            double covariance = sumWeights2 - 2.0 * pf * sumWeights + nDirections * pf * pf;
            covariance = std::sqrt(covariance / nDirections / (nDirections - 1.0));
            double qConvergence = covariance / pf;
            double pConvergence = covariance / (1.0 - pf);

            double convergence = std::max(qConvergence, pConvergence);

            return convergence;
        }

        std::vector<double> DirectionalSampling::getDirectionBetas(Models::ModelRunner& modelRunner,
            const std::vector<std::shared_ptr<Models::Sample>>& samples, double z0, double threshold)
        {
            const size_t nSamples = samples.size();
            auto betaValues = std::vector<double>(nSamples);

            std::vector<DirectionReliabilityDS> directions;
            for (auto& sample : samples)
            {
                directions.emplace_back(threshold, z0, *Settings->DirectionSettings, *sample);
            }

            if ( ! modelRunner.canCalculateBeta())
            {
                auto shouldCompute = std::vector(nSamples, true);
                if (modelRunner.ProxySettings->IsProxyModel)
                {
                    for (size_t i = 0; i < nSamples; i++)
                    {
                        // retain previous results from model if running in a proxy model environment
                        shouldCompute[i] = !previousResults.contains(samples[i]->IterationIndex);
                    }
                }

                auto preComputeDirs = PrecomputeDirections(*Settings->DirectionSettings, z0);
                preComputeDirs.precompute(modelRunner, directions, shouldCompute);
                preComputedCounter += preComputeDirs.GetCounter();
            }

            const double z0Fac = getZFactor(z0);

            #pragma omp parallel for
            for (int i = 0; i < static_cast<int>(nSamples); i++)
            {
                // retain previous results from model if running in a proxy model environment
                if (modelRunner.ProxySettings->IsProxyModel && previousResults.contains(samples[i]->IterationIndex))
                {
                    betaValues[i] = previousResults[samples[i]->IterationIndex];
                }
                else
                {
                    samples[i]->threadId = omp_get_thread_num();
                    betaValues[i] = directions[i].getBeta(modelRunner, z0Fac);
                }
            }

            // keep results from non-proxy runs when proxies are used for a next run
            if (modelRunner.ProxySettings->IsProxyModel)
            {
                for (size_t i = 0; i < samples.size(); i++)
                {
                    if (!samples[i]->AllowProxy)
                    {
                        previousResults.insert({ samples[i]->IterationIndex, betaValues[i] });
                    }
                }
            }

            return betaValues;
        }
    }
}



