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
#include "ImportanceSamplingS.h"
#include <vector>
#include <cmath>
#include <memory>

#include "../Model/Sample.h"
#include "../Model/RandomSampleGenerator.h"
#include "../Math/NumericSupport.h"
#include "ImportanceSamplingSettingsS.h"

using namespace Deltares::Models;

namespace Deltares
{
    namespace Sensitivity
    {
        std::shared_ptr<Sensitivity::SensitivityResult> ImportanceSamplingS::getSensitivityStochast(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            std::shared_ptr<SampleProvider> sampleProvider = std::make_shared<SampleProvider>(this->Settings->StochastSet, false);
            modelRunner->setSampleProvider(sampleProvider);

            const std::shared_ptr<RandomSampleGenerator> randomSampleGenerator = std::make_shared<RandomSampleGenerator>();
            randomSampleGenerator->Settings = this->Settings->randomSettings;
            randomSampleGenerator->Settings->StochastSet = this->Settings->StochastSet;
            randomSampleGenerator->sampleProvider = sampleProvider;
            randomSampleGenerator->initialize();

            int nParameters = modelRunner->getVaryingStochastCount();
            std::vector<double> zValues; // copy of z for all parallel threads as double

            std::shared_ptr<Sample> uMin = std::make_shared<Sample>(nParameters);
            std::vector<std::shared_ptr<Sample>> samples;
            std::vector<double> zSamples;
            std::vector<double> zWeights;
            size_t zIndex = 0;
            int nSamples = 0;
            double sumWeights = 0;

            bool registerSamplesForCorrelation = this->correlationMatrixBuilder->isEmpty() &&
                                                 this->Settings->CalculateCorrelations &&
                                                 this->Settings->CalculateInputCorrelations;

            std::shared_ptr<Sample> center = Settings->StochastSet->getStartPoint();

            std::vector<double> factors = this->getFactors(Settings->StochastSet);
            double dimensionality = getDimensionality(factors);

            bool converged = false;

            for (int sampleIndex = 0; sampleIndex < Settings->MaximumSamples && !converged && !isStopped(); sampleIndex++)
            {
                zIndex++;

                if (zIndex >= samples.size())
                {
                    samples.clear();

                    int chunkSize = modelRunner->Settings->MaxChunkSize;
                    int runs = std::min(chunkSize, Settings->MaximumSamples - sampleIndex);

                    sampleProvider->reset();

                    for (int i = 0; i < runs; i++)
                    {
                        std::shared_ptr<Sample> sample = randomSampleGenerator->getRandomSample();
                        std::shared_ptr<Sample> modifiedSample = sample->clone();

                        for (int k = 0; k < nParameters; k++)
                        {
                            modifiedSample->Values[k] = factors[k] * sample->Values[k] + center->Values[k];
                        }

                        modifiedSample->Weight = getWeight(modifiedSample, sample, dimensionality);

                        samples.push_back(modifiedSample);
                    }

                    zValues = modelRunner->getZValues(samples);

                    zIndex = 0;
                }

                double z = zValues[zIndex];
                std::shared_ptr<Sample> u = samples[zIndex];

                if (std::isnan(z))
                {
                    continue;
                }

                zSamples.push_back(z);
                zWeights.push_back(samples[zIndex]->Weight);
                if (registerSamplesForCorrelation)
                {
                    modelRunner->registerSample(this->correlationMatrixBuilder, samples[zIndex]);
                }

                sumWeights += samples[zIndex]->Weight;
                nSamples++;

                // check if convergence is reached (or stop criterion)
                if (sampleIndex >= Settings->MinimumSamples)
                {
                    double convergence = getConvergence(sampleIndex, sumWeights);
                    converged = convergence < this->Settings->VariationCoefficient;
                }
            }

            // adjust for weights not equal to number of samples,
            // only adjust samples with weight > 1, because they will not be in the tail of the distribution, which is the less interesting part

            double weightDifference = nSamples - sumWeights;
            double overWeightedSum = 0;

            // calculate over weight frm samples with weight > 1
            for (size_t i = 0; i < zWeights.size(); i++)
            {
                if (zWeights[i] > 1)
                {
                    overWeightedSum += zWeights[i];
                }
            }

            // correct samples with weight > 1 so that sum of weights equals number of samples
            for (size_t i = 0; i < zWeights.size(); i++)
            {
                if (zWeights[i] > 1)
                {
                    zWeights[i] += zWeights[i] * weightDifference / overWeightedSum;
                }
            }

            std::shared_ptr<Statistics::Stochast> stochast = this->getStochastFromSamples(zSamples, zWeights);

            if (this->correlationMatrixBuilder != nullptr)
            {
                this->correlationMatrixBuilder->registerWeights(zWeights);
                this->correlationMatrixBuilder->registerSamples(stochast, zSamples);
            }

            return modelRunner->getSensitivityResult(stochast);
        }

        /// <summary>
        /// return the weight for the sample.
        /// This is given by: dimensionality * pdf / pdfOriginal, with pdf = normalFactor * std::exp(-SquaredSum/2).
        /// Rewritten to avoid a possible division by zero.
        /// </summary>
        /// <param name="modifiedSample"> the scaled sample </param>
        /// <param name="sample"> the original sample </param>
        /// <param name="dimensionality"> constant dependent on the number of stochasts </param>
        /// <returns> the weight </returns>
        double ImportanceSamplingS::getWeight(std::shared_ptr<Sample> modifiedSample, std::shared_ptr<Sample> sample, double dimensionality)
        {
            double sumSquared = Numeric::NumericSupport::GetSquaredSum(modifiedSample->Values);
            double sumSquaredOrg = Numeric::NumericSupport::GetSquaredSum(sample->Values);
            double ratioPdf = std::exp(0.5 * (sumSquaredOrg - sumSquared));
            return dimensionality * ratioPdf;
        }

        double ImportanceSamplingS::getDimensionality(std::vector<double> factors)
        {
            double dimensionality = 1; // correction for the dimensionality effect

            for (size_t k = 0; k < factors.size(); k++)
            {
                dimensionality *= factors[k];
            }

            return dimensionality;
        }

        std::vector<double> ImportanceSamplingS::getFactors(std::shared_ptr<Reliability::StochastSettingsSet> stochastSettings)
        {
            std::vector<double> factors(stochastSettings->getVaryingStochastCount());

            for (size_t k = 0; k < factors.size(); k++)
            {
                factors[k] = stochastSettings->VaryingStochastSettings[k]->VarianceFactor;
            }

            return factors;
        }

        double ImportanceSamplingS::getConvergence(int samples, double weightedSum)
        {
            double nSamples = samples;

            //TODO: PROBL-42 check whether this procedure is correct

            double nLow = Settings->ProbabilityForConvergence * weightedSum;
            double pLow = nLow / nSamples;
            if (pLow > 0.5)
            {
                pLow = 1 - pLow;
            }

            double nHigh = (1 - Settings->ProbabilityForConvergence) * weightedSum;
            double pHigh = nHigh / nSamples;
            if (pHigh > 0.5)
            {
                pHigh = 1 - pHigh;
            }

            double pf = std::min(pLow, pHigh);

            double varPf = std::sqrt((1 - pf) / (nSamples * pf));

            return varPf;
        }
    }
}



