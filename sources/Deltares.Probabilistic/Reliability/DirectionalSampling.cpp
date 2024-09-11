// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "DirectionalSampling.h"
#include "DirectionReliability.h"
#include "../Model/RandomSampleGenerator.h"
#include "../Math/SpecialFunctions.h"
#include <omp.h>

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> DirectionalSampling::getDesignPoint(std::shared_ptr<ModelRunner> modelRunner)
        {
            int nstochasts = modelRunner->getVaryingStochastCount();
            double pf = 0;
            double qtot = 0;
            double rmin = 200; // initialise convergence indicator and loops
            std::shared_ptr<DesignPointBuilder> uMean = std::make_shared<DesignPointBuilder> (nstochasts, this->Settings->designPointMethod, this->Settings->StochastSet);
            int parSamples = 0;

            std::vector<double> betaValues;
            std::vector<std::shared_ptr<Sample>> samples;

            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

            std::shared_ptr<RandomSampleGenerator> randomSampleGenerator = std::make_shared<RandomSampleGenerator>();
            randomSampleGenerator->Settings = this->Settings->randomSettings;
            randomSampleGenerator->Settings->StochastSet = this->Settings->StochastSet;
            randomSampleGenerator->initialize();

            std::shared_ptr<Sample> zeroSample = std::make_shared<Sample>(nstochasts);
            double z0 = modelRunner->getZValue(zeroSample);
            double z0Fac = this->getZFactor(z0);

            std::shared_ptr<Sample> uMin = std::make_shared<Sample>(nstochasts);
            int chunkSize = modelRunner->Settings->MaxChunkSize;

            double sumPfSamp = 0.0; double sumPfSamp2 = 0.0;
            int validSamples = 0;

            if (modelRunner->Settings->MaxParallelProcesses > 0)
            {
                omp_set_num_threads(modelRunner->Settings->MaxParallelProcesses);
            }

            // loop for all directions
            for (int nmaal = 0; nmaal < Settings->MaximumDirections && !isStopped(); nmaal++)
            {
                if (nmaal % chunkSize == 0)
                {
                    samples.clear();

                    int runs = std::min(chunkSize, Settings->MaximumDirections - parSamples * chunkSize);

                    // run max par samples times zrfunc in parallel
                    for (int i = 0; i < runs; i++)
                    {
                        std::shared_ptr<Sample> sample = randomSampleGenerator->getRandomSample();
                        sample->IterationIndex = nmaal + i;
                        samples.push_back(sample);
                    }

                    betaValues = getDirectionBetas(modelRunner, samples, z0Fac, rmin);

                    // check whether restart is needed
                    if (modelRunner->shouldExitPrematurely(samples))
                    {
                        // return the result so far
                        return modelRunner->getDesignPoint(uMean->getSample(), Statistics::StandardNormal::getUFromQ(pf), convergenceReport);
                    }

                    parSamples = parSamples + 1;
                }

                if (betaValues[nmaal % chunkSize] * z0 < 0 || std::isnan(betaValues[nmaal % chunkSize]))
                {
                    continue;
                }

                std::shared_ptr<Sample> u = samples[nmaal % chunkSize];

                double betaDirection = std::abs(betaValues[nmaal % chunkSize]);

                // get the sample at the limit state
                std::shared_ptr<Sample> uSurface = u->getSampleAtBeta(betaDirection);

                // calculate failure probability
                if (betaDirection >= 0 && betaDirection < Statistics::StandardNormal::BetaMax * nstochasts)
                {
                    uSurface->Weight = Deltares::Numeric::SpecialFunctions::getGammaUpperRegularized(0.5 * nstochasts, 0.5 * betaDirection * betaDirection);

                    qtot += uSurface->Weight;

                    if (uSurface->Weight > 0)
                    {
                        int k = 1;
                    }

                    uMean->addSample(uSurface);
                }
                else
                {
                    uSurface->Weight = 0;
                }

                validSamples++;

                pf = qtot / (double)validSamples;

                // store minimum value of r and alpha
                if (betaDirection < rmin)
                {
                    rmin = betaDirection;
                    uMin = uSurface;
                }

                // check on convergence criterium
                bool enoughSamples = nmaal >= Settings->MinimumDirections;
                convergenceReport->TotalDirections = nmaal+1;

                sumPfSamp += uSurface->Weight;
                sumPfSamp2 += pow(uSurface->Weight, 2);

                if (qtot > 0)
                {
                    // determine accuracy

                    double convergence = getConvergence(pf, sumPfSamp, sumPfSamp2, (double)validSamples);

                    double beta = z0Fac * Statistics::StandardNormal::getUFromQ(pf);

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

            convergenceReport->Convergence = getConvergence(pf, sumPfSamp, sumPfSamp2, (double)validSamples);
            convergenceReport->IsConverged = (convergenceReport->Convergence <= Settings->VariationCoefficient);

            std::shared_ptr<Sample> uDesign = uMean->getSample();

            if (z0 < 0)
            {
                pf = 1 - pf;
            }

            samples.clear();

            return modelRunner->getDesignPoint(uDesign, Statistics::StandardNormal::getUFromQ(pf), convergenceReport);
        }

        double DirectionalSampling::getConvergence(const double pf, const double sumPfSamp, const double sumPfSamp2, const double dTimes)
        {
            double covar = sumPfSamp2 - 2.0 * pf * sumPfSamp + dTimes * pow(pf, 2);
            covar = std::sqrt(covar / dTimes / (dTimes - 1.0));
            double covarpf = covar / pf;
            double covarnpf = covar / (1 - pf);

            double convergence = std::max(covarpf, covarnpf);

            return convergence;
        }

        std::vector<double> DirectionalSampling::getDirectionBetas(std::shared_ptr<Models::ModelRunner> modelRunner, std::vector<std::shared_ptr<Sample>> samples, double z0, double threshold)
        {
            auto betaValues = std::vector<double>(samples.size());

            std::unique_ptr<DirectionReliabilityForDirectionalSampling> directionReliability = std::make_unique<DirectionReliabilityForDirectionalSampling>();
            directionReliability->Settings = this->Settings->DirectionSettings;
            directionReliability->Threshold = threshold;

            #pragma omp parallel for
            for (int i = 0; i < (int)samples.size(); i++)
            {
                samples[i]->threadId = omp_get_thread_num();
                // retain previous results from model if running in a proxy model environment
                if (modelRunner->Settings->IsProxyModel() && previousResults.contains(samples[i]->IterationIndex))
                {
                    betaValues[i] = previousResults[samples[i]->IterationIndex];
                }
                else 
                {
                    betaValues[i] = directionReliability->getBeta(modelRunner, samples[i], z0);
                }
            }

            // keep results from non proxy runs when proxies are used for a next run
            if (modelRunner->Settings->IsProxyModel())
            {
                for (size_t i = 0; i < samples.size(); i++)
                {
                    if (!samples[i]->AllowProxy)
                    {
                        this->previousResults.insert({ samples[i]->IterationIndex, betaValues[i] });
                    }
                }
            }

            return betaValues;
        }
    }
}



