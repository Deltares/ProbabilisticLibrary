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
#include "SubsetSimulation.h"

#include <algorithm>
#include <numbers>

#include "DesignPoint.h"
#include "DesignPointBuilder.h"
#include "../Math/NumericSupport.h"
#include "../Statistics/StandardNormal.h"
#include "../Model/RandomSampleGenerator.h"

#if __has_include(<format>)
#include <format>
#else
#include "../Utils/probLibString.h"
#endif


namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Numeric;

        std::shared_ptr<DesignPoint> SubsetSimulation::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            int nStochasts = modelRunner->getVaryingStochastCount();

            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            std::shared_ptr<DesignPointBuilder> designPointBuilder = std::make_shared<DesignPointBuilder>(nStochasts, Settings->designPointMethod, Settings->StochastSet);
            randomSampleGenerator = std::make_shared<RandomSampleGenerator>(this->Settings->randomSettings, this->Settings->StochastSet);
            randomSampleGenerator->initialize();

            // initialize convergence indicator and loops
            double ssFactor = 1;
            double pf = 0;

            bool initial = true;
            bool ready = false;
            double z0Fac = 0;
            int iteration = 0;

            std::shared_ptr<ConvergenceReport> convergenceReport = nullptr;
            std::vector<std::shared_ptr<DesignPoint>> contributingDesignPoints;
            std::vector<std::shared_ptr<Sample>> selectedSamples;

            while (!ready && !isStopped())
            {
                modelRunner->clear();

                int samplesCount = 0;
                int zIndex = 0;
                bool converged = false;
                int ntot = 0;

                this->rejectedSamples = 0;
                this->acceptedSamples = 0;

                convergenceReport = std::make_shared<ConvergenceReport>();

                iteration++;

                std::vector<std::shared_ptr<Sample>> samples;
                std::vector<std::shared_ptr<Sample>> performedSamples;
                std::vector<double> zValues;

                // create samples
                std::vector<std::shared_ptr<Sample>> newSamples = getNewSamples(modelRunner, randomSampleGenerator, initial, z0Fac, selectedSamples);

                int skip = 0;

                for (int sampleIndex = 0; sampleIndex < newSamples.size() && !converged && !isStopped(); sampleIndex++)
                {
                    zIndex++;

                    if (initial || zIndex >= zValues.size())
                    {
                        zIndex = 0;

                        // select the new samples
                        samples.clear();
                        int chunkSize = modelRunner->Settings->MaxChunkSize;
                        int runs = std::min(chunkSize, (int)newSamples.size() - sampleIndex);

                        for (int i = skip; i < skip + runs; i++)
                        {
                            samples.push_back(newSamples[i]);
                        }

                        skip += runs;

                        // retrieve sample results

                        if (selectedSamples.empty())
                        {
                            zValues = modelRunner->getZValues(samples);
                        }
                        else
                        {
                            zValues = Sample::select(samples, [](std::shared_ptr<Sample> p) { return p->Z; });
                        }

                        // determine multiplication factor for z (z<0) at u = 0
                        if (initial)
                        {
                            z0Fac = this->getZFactor(zValues[zIndex]);
                            initial = false;
                            continue;
                        }

                        if (modelRunner->shouldExitPrematurely(samples))
                        {
                            // return the result so far
                            return modelRunner->getDesignPoint(designPointBuilder->getSample(), Statistics::StandardNormal::getUFromQ(pf), convergenceReport);
                        }
                    }

                    double z = zValues[zIndex] * z0Fac;

                    std::shared_ptr<Sample> sample = samples[zIndex];
                    performedSamples.push_back(samples[zIndex]);

                    // ignore a failed evaluation
                    if (std::isnan(z))
                    {
                        continue;
                    }

                    // increase the number of succeeded samples
                    samplesCount++;

                    // determine total probability of failure
                    if (z < 0)
                    {
                        ntot = ntot + 1;
                    }

                    convergenceReport->FailedSamples = ntot;
                    convergenceReport->FailFraction = NumericSupport::Divide(ntot, samplesCount);

                    // register minimum value of r and alpha
                    if (z * z0Fac < 0)
                    {
                        designPointBuilder->addSample(sample);
                    }

                    // uncorrected prob of failure for convergence
                    pf = NumericSupport::Divide(ntot, samplesCount);

                    std::shared_ptr<ReliabilityReport> report = std::make_shared<ReliabilityReport>();
                    report->Step = sampleIndex;
                    report->MaxSteps = Settings->MaximumSamples;

                    if (pf > 0 && pf < 1.0)
                    {
                        // check if convergence is reached (or stop criterion)
                        convergenceReport->Convergence = getConvergence(pf, samplesCount);
                        convergenceReport->IsConverged = isConverged(sampleIndex, convergenceReport->Convergence);

                        converged = convergenceReport->IsConverged;

                        report->Reliability = z0Fac * Statistics::StandardNormal::getUFromQ(pf * ssFactor);
                        report->Variation = convergenceReport->Convergence;

                        modelRunner->reportResult(report);

                    }
                    else
                    {
                        modelRunner->reportResult(report);
                    }
                }

                // determine probability and beta if variation is large
                pf = ssFactor * NumericSupport::Divide(ntot, samplesCount);

                if (!selectedSamples.empty())
                {
                    const double ratio = 100 * NumericSupport::Divide(rejectedSamples, acceptedSamples + rejectedSamples);
#if __has_include(<format>)
                    std::string message = std::format("Rejected {0:} values, accepted {1:} values ({2:} % rejected) in iteration {3:}", rejectedSamples, acceptedSamples, ratio, iteration);
#else
                    std::string message = "Rejected " + std::to_string(rejectedSamples) + ", accepted " + std::to_string(acceptedSamples) + " samples";
#endif
                    modelRunner->reportMessage(MessageType::Debug, message);
                }

                // select samples for the next round
                selectedSamples = selectSamples(z0Fac, performedSamples);

                ssFactor *= Settings->SubsetFraction;

                ready = ssFactor < 1E-20 || selectedSamples.back()->Z * z0Fac < 0;

                if (!ready)
                {
                    std::shared_ptr<Sample> sample = designPointBuilder->getSample();
                    std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(sample, Statistics::StandardNormal::getUFromQ(pf), convergenceReport);
#if __has_include(<format>)
                    std::string identifier = std::format("Subset iteration {0:}", iteration);
#else
                    std::string identifier = "Subset iteration " + std::to_string(iteration);
#endif
                    designPoint->Identifier = identifier;

                    contributingDesignPoints.push_back(designPoint);
                }
            }

            std::shared_ptr<Sample> sample = designPointBuilder->getSample();
            std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(sample, z0Fac * Statistics::StandardNormal::getUFromQ(pf), convergenceReport);

            for (size_t i = 0; i < contributingDesignPoints.size(); i++)
            {
                designPoint->ContributingDesignPoints.push_back(contributingDesignPoints[i]);
            }

            return designPoint;
        }

        std::vector<std::shared_ptr<Sample>> SubsetSimulation::getNewSamples(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<RandomSampleGenerator> randomSampleGenerator, bool initial, double z0Fac, std::vector<std::shared_ptr<Sample>> selectedSamples)
        {
            if (selectedSamples.empty())
            {
                return getInitialSamples(modelRunner, randomSampleGenerator, initial);
            }
            else if (Settings->SampleMethod == SampleMethodType::MarkovChain)
            {
                return getMarkovChainSamples(modelRunner, selectedSamples, z0Fac);
            }
            else if (Settings->SampleMethod == SampleMethodType::AdaptiveConditional)
            {
                return getAdaptiveConditionalSamples(modelRunner, selectedSamples);
            }
            else
            {
                throw probLibException("Sample method type");
            }
        }

        std::vector<std::shared_ptr<Sample>> SubsetSimulation::getInitialSamples(std::shared_ptr<ModelRunner> modelRunner, std::shared_ptr<RandomSampleGenerator> randomSampleGenerator, bool initial)
        {
            std::vector<std::shared_ptr<Sample>> samples;

            if (initial)
            {
                samples.push_back(std::make_shared<Sample>(modelRunner->getVaryingStochastCount()));
            }

            for (int i = 0; i < Settings->MaximumSamples; i++)
            {
                // initial run, take random samples
                std::shared_ptr<Sample> sample = randomSampleGenerator->getRandomSample();
                sample->IterationIndex = -1;

                samples.push_back(sample);
            }

            return samples;
        }

        std::vector<std::shared_ptr<Sample>> SubsetSimulation::getMarkovChainSamples(std::shared_ptr<ModelRunner> modelRunner, std::vector<std::shared_ptr<Sample>>& selectedSamples, double z0Fac)
        {
            std::vector<std::shared_ptr<Sample>> samples;

            double maxZ = selectedSamples.back()->Z;

            for (int i = 0; i < this->Settings->MaximumSamples; i++)
            {
                samples.push_back(getMarkovChainSample(selectedSamples[i % selectedSamples.size()], modelRunner, maxZ, z0Fac));
            }

            return samples;
        }

        std::shared_ptr<Sample> SubsetSimulation::getMarkovChainSample(std::shared_ptr<Sample> oldSample, std::shared_ptr<ModelRunner> modelRunner, double maxZ, double z0Fac)
        {
            constexpr int maxTries = 10;

            int tries = 0;

            while (tries++ < maxTries)
            {
                bool allRejected = true;

                std::vector<double> newValues(oldSample->Values.size());

                for (int i = 0; i < oldSample->Values.size(); i++)
                {
                    const double random = randomSampleGenerator->random.next();

                    const double newValue = oldSample->Values[i] + (2 * random - 1) * Settings->MarkovChainDeviation;
                    const double oldDensity = getStandardNormalPDF(oldSample->Values[i]);
                    const double newDensity = getStandardNormalPDF(newValue);

                    const double acceptanceRatio = std::min(1.0, newDensity / oldDensity);

                    if (acceptanceRatio > randomSampleGenerator->random.next())
                    {
                        acceptedSamples++;
                        newValues[i] = newValue;
                        allRejected = false;
                    }
                    else
                    {
                        rejectedSamples++;
                        newValues[i] = oldSample->Values[i];
                    }
                }

                std::shared_ptr<Sample> sample = std::make_shared<Sample>(newValues);

                if (!allRejected)
                {
                    double z = modelRunner->getZValue(sample);

                    if (z * z0Fac <= maxZ)
                    {
                        return sample;
                    }
                }
            }

            // calculate again, so that evaluation is logged
            double oldZ = modelRunner->getZValue(oldSample);

            return oldSample;
        }

        std::vector<std::shared_ptr<Sample>> SubsetSimulation::getAdaptiveConditionalSamples(std::shared_ptr<ModelRunner> modelRunner, std::vector<std::shared_ptr<Sample>>& selectedSamples)
        {
            double b = selectedSamples.back()->Z;

            size_t nStochasts = modelRunner->getVaryingStochastCount(); //number of parameters
            size_t nSelectedSamples = selectedSamples.size(); // number of seeds

            size_t nMaximumSamples = Settings->MaximumSamples;

            int nChains = static_cast<int>(std::ceil(100.0 * nSelectedSamples / nMaximumSamples)); // number of chains after which the proposal is adapted

            constexpr double oldLambda = 0.60;

            // initialization
            std::vector<int> acceptance(Settings->MaximumSamples); // store acceptance
            auto number_of_levels = static_cast<int>(std::floor(static_cast<double>(nSelectedSamples) / static_cast<double>(nChains))) + 1;
            std::vector<double> lam(number_of_levels); // scaling parameter \in (0,1)
            std::vector<double> mu_acc(number_of_levels);
            std::vector<double> hat_a(number_of_levels); // average acceptance rate of the chains

            //  number of samples per chain
            auto default_nChain = static_cast<int>(std::floor(static_cast<double>(nMaximumSamples) / static_cast<double>(nSelectedSamples)));
            std::vector<int> nChain(nSelectedSamples, default_nChain);
            for (size_t j = 0; j < nMaximumSamples % nSelectedSamples; j++)
            {
                nChain[j]++;
            }

            // The arrays begin with initial value 0
            std::vector<std::shared_ptr<Sample>> newSamples;

            // 1. compute the standard deviation

            std::vector<double> sigma_0(nStochasts, 1.0);

            // 2. iteration
            lam[0] = oldLambda; //  initial scaling parameter \in (0,1)
            std::vector<double> sigma(nStochasts, oldLambda);
            std::vector<double> rho(nStochasts, std::sqrt(1.0 - oldLambda * oldLambda));

            //a.compute correlation parameter

            size_t i = 0; // index for adaptation of lambda

            // b. apply conditional sampling

            for (size_t k = 1; k < nSelectedSamples + 1; k++)
            {
                int idx = 0;
                for (size_t j = 0; j < k - 1; j++)
                {
                    idx += nChain[j];
                } // beginning of each chain index

                std::shared_ptr<Sample> newSample = selectedSamples[k - 1]->clone();
                newSample->Z = selectedSamples[k - 1]->Z;

                newSamples.push_back(newSample);

                std::shared_ptr<Sample> previousSample = newSample;

                for (int t = 1; t < nChain[k - 1]; t++)
                {
                    std::vector<double> values(nStochasts);

                    for (size_t j = 0; j < nStochasts; j++)
                    {
                        double random = randomSampleGenerator->random.next();
                        double u = Statistics::StandardNormal::getUFromQ(random);
                        values[j] = previousSample->Values[j] * rho[j] + sigma[j] * u;
                    }

                    std::shared_ptr<Sample> sample = std::make_shared<Sample>(values);

                    sample->Z = modelRunner->getZValue(sample);

                    //  accept or reject sample 
                    if (sample->Z <= b)
                    {
                        acceptance[idx + t] = 1; //  note the acceptance
                    }
                    else
                    {
                        sample = previousSample->clone();
                        sample->Z = previousSample->Z;

                        acceptance[idx + t] = 0; // note the rejection
                    }

                    newSamples.push_back(sample);

                    previousSample = sample;
                } // end of for-t loop

                // average of the accepted samples for each seed 'mu_acc'
                // here the warning "Mean of empty slice" is not an issue

                double mean = 0.0;
                for (int j = idx + 1; j < idx + nChain[k - 1]; j++)
                {
                    mean += acceptance[j];
                }

                mean /= nChain[k - 1];

                mu_acc[i] += std::min(1.0, mean);

                if (k % nChains == 0)
                {
                    if (nChain[k - 1] > 1)
                    {
                        constexpr double star_a = 0.44; // optimal acceptance rate

                        // c. evaluate average acceptance rate

                        hat_a[i] = mu_acc[i] / static_cast<double>(nChains); // Ref. 1 Eq. 25

                        // d. compute new scaling parameter
                        double zeta = 1.0 / std::sqrt(static_cast<double>(i + 1)); //  ensures that the variation of lambda(i) vanishes
                        lam[i + 1] = std::exp(std::log(lam[i]) + zeta * (hat_a[i] - star_a)); //  Ref. 1 Eq. 26

                        // update parameters
                        for (size_t j = 0; j < nStochasts; j++)
                        {
                            sigma[j] = std::min(1.0, lam[i + 1] * sigma_0[j]); //  Ref. 1 Eq. 23
                            rho[j] = std::sqrt(1.0 - sigma[j] * sigma[j]); //  Ref. 1 Eq. 24
                        }

                        // update counter
                        i++;

                    } // end if

                } // end if ( k % nChains == 0 )

            } // end of for-k loop

            // compute mean acceptance rate of all chains

            if (i != 0)
            {
                double mean = 0.0;
                int num = 1;
                for (size_t j = 0; j < i - 1; j++)
                {
                    mean += hat_a[j];
                    num++;
                }

                acceptanceRate = mean / num;
            }
            else // no adaptation
            {
                acceptanceRate = 0;

                if (nMaximumSamples % nSelectedSamples != 0)
                {
                    int num = 1;
                    for (size_t j = 0; j < nMaximumSamples % nSelectedSamples; j++)
                    {
                        acceptanceRate += acceptance[j];
                        num++;
                    }
                    acceptanceRate /= num; // only for the few generated samples
                }
            }

            return newSamples;
        }

        std::vector<std::shared_ptr<Sample>> SubsetSimulation::selectSamples(double z0Fac, std::vector<std::shared_ptr<Sample>> performedSamples)
        {
            std::sort(performedSamples.begin(), performedSamples.end(),
                [z0Fac](const std::shared_ptr<Sample>& val1, const std::shared_ptr<Sample>& val2)
                {return val1->Z * z0Fac < val2->Z * z0Fac; });

            std::vector<std::shared_ptr<Sample>> selectedSamples;

            for (int i = 0; i < Settings->SubsetFraction * performedSamples.size(); i++)
            {
                selectedSamples.push_back(performedSamples[i]);
            }

            return selectedSamples;
        }

        double SubsetSimulation::getConvergence(double pf, int samples)
        {
            if (pf > 0 && pf < 1)
            {
                if (pf > 0.5)
                {
                    pf = 1 - pf;
                }
                double varPf = sqrt((1 - pf) / (samples * pf));
                return varPf;
            }
            else
            {
                return nan("");
            }
        }

        bool SubsetSimulation::isConverged(int sampleIndex, double convergence) const
        {
            double requiredConvergence = Settings->SampleMethod == SampleMethodType::AdaptiveConditional ? 0 : Settings->VariationCoefficient;
            return sampleIndex >= Settings->MinimumSamples && convergence < requiredConvergence;
        }

        double SubsetSimulation::getStandardNormalPDF(double u)
        {
            double normalFactor = 1 / std::sqrt(2 * std::numbers::pi);

            double distance = -u * u / 2;

            return normalFactor * std::exp(distance);
        }

    }
}

