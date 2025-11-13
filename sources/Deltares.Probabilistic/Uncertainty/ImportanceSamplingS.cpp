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
#include "ImportanceSamplingSettingsS.h"
#include "../Reliability/DesignPointBuilder.h"
#include "../Reliability/ImportanceSamplingSupport.h"

using namespace Deltares::Models;

namespace Deltares::Uncertainty
{
    UncertaintyResult ImportanceSamplingS::getUncertaintyStochast(std::shared_ptr<Models::ModelRunner> modelRunner)
    {
        modelRunner->updateStochastSettings(this->Settings->StochastSet);

        std::shared_ptr<SampleProvider> sampleProvider = std::make_shared<SampleProvider>(this->Settings->StochastSet);
        modelRunner->setSampleProvider(sampleProvider);

        auto randomSampleGenerator = RandomSampleGenerator();
        randomSampleGenerator.Settings = this->Settings->randomSettings;
        randomSampleGenerator.Settings->StochastSet = this->Settings->StochastSet;
        randomSampleGenerator.sampleProvider = sampleProvider;
        randomSampleGenerator.initialize();

        int nParameters = modelRunner->getVaryingStochastCount();

        // for convergence, simulate that this is a reliability calculation with failure at given probability
        Reliability::DesignPointBuilder designPointBuilder = Reliability::DesignPointBuilder(nParameters, Reliability::CenterOfGravity, this->Settings->StochastSet);
        double normalizedProbabilityForConvergence = std::min(Settings->ProbabilityForConvergence, 1 - Settings->ProbabilityForConvergence);

        std::shared_ptr<Sample> uMin = std::make_shared<Sample>(nParameters);
        std::vector<std::shared_ptr<Sample>> samples;
        std::vector<double> zValues;
        std::vector<double> weights;
        std::vector<double> cumulativeWeights;
        std::vector<bool> isDesignPointSamples;
        size_t zIndex = 0;
        int nSamples = 0;
        double sumWeights = 0;
        std::vector<std::shared_ptr<Sample>> allSamples;

        bool registerSamplesForCorrelation = this->correlationMatrixBuilder->isEmpty() &&
            this->Settings->CalculateCorrelations &&
            this->Settings->CalculateInputCorrelations;

        std::shared_ptr<Sample> center = Settings->StochastSet->getStartPoint();

        std::vector<double> factors = this->getFactors(Settings->StochastSet);
        double dimensionality = Reliability::ImportanceSamplingSupport::getDimensionality(factors);

        bool converged = false;

        for (int sampleIndex = 0; sampleIndex < Settings->MaximumSamples && !converged && !isStopped(); sampleIndex++)
        {
            zIndex++;

            if (zIndex >= samples.size())
            {
                samples.clear();

                int chunkSize = modelRunner->Settings->MaxChunkSize;
                int runs = std::min(chunkSize, Settings->MaximumSamples - sampleIndex);

                for (int i = 0; i < runs; i++)
                {
                    std::shared_ptr<Sample> sample = randomSampleGenerator.getRandomSample();
                    std::shared_ptr<Sample> modifiedSample = getModifiedSample(sample, factors, center, dimensionality);
                    samples.push_back(modifiedSample);
                }

                std::vector<double> zSampleValues = modelRunner->getZValues(samples);

                zIndex = 0;
            }

            std::shared_ptr<Sample> sample = samples[zIndex];

            if (std::isnan(sample->Z))
            {
                continue;
            }

            zValues.push_back(sample->Z);
            weights.push_back(sample->Weight);
            sumWeights += sample->Weight;
            isDesignPointSamples.push_back(false);
            cumulativeWeights.push_back(sample->Weight);
            allSamples.push_back(sample);

            nSamples++;

            updateCumulativeWeights(zValues, weights, cumulativeWeights, sample);

            if (registerSamplesForCorrelation)
            {
                modelRunner->registerSample(this->correlationMatrixBuilder, sample);
            }

            // check whether all added samples in design point builder are supposed to be there
            // the design point builder should contain samples corresponding with a given probability of failure
            for (size_t i = 0; i < cumulativeWeights.size(); i++)
            {
                bool shouldBeDesignPointSample = cumulativeWeights[i] < sumWeights * normalizedProbabilityForConvergence;
                if (shouldBeDesignPointSample && !isDesignPointSamples[i])
                {
                    // register for counting in design point
                    designPointBuilder.addSample(allSamples[i]);
                    isDesignPointSamples[i] = true;
                }
                else if (!shouldBeDesignPointSample && isDesignPointSamples[i])
                {
                    // unregister for counting in design point
                    designPointBuilder.removeSample(allSamples[i]);
                    isDesignPointSamples[i] = false;
                }
            }

            // check if convergence is reached (or stop criterion)
            if (sampleIndex >= Settings->MinimumSamples)
            {
                std::shared_ptr<Sample> designPoint = designPointBuilder.getSample();

                double designPointWeight = Reliability::ImportanceSamplingSupport::getSampleWeight(designPoint, center, dimensionality, factors);

                double convergence = Reliability::ImportanceSamplingSupport::getConvergence(normalizedProbabilityForConvergence, designPointWeight, nSamples);
                converged = convergence < this->Settings->VariationCoefficient;
            }
        }

        // adjust for weights not equal to number of samples,
        // only adjust samples with weight > 1, because they will not be in the tail of the distribution, which is the less interesting part

        double weightDifference = nSamples - sumWeights;
        double overWeightedSum = 0;

        // calculate over weight frm samples with weight > 1
        for (size_t i = 0; i < weights.size(); i++)
        {
            if (weights[i] > 1)
            {
                overWeightedSum += weights[i];
            }
        }

        // correct samples with weight > 1 so that sum of weights equals number of samples
        for (size_t i = 0; i < weights.size(); i++)
        {
            if (weights[i] > 1)
            {
                weights[i] += weights[i] * weightDifference / overWeightedSum;
            }
        }

        std::shared_ptr<Statistics::Stochast> stochast = this->getStochastFromSamples(zValues, weights);

        auto result = modelRunner->getUncertaintyResult(stochast);

        for (std::shared_ptr<Statistics::ProbabilityValue> quantile : this->Settings->RequestedQuantiles)
        {
            double p = quantile->getProbabilityOfNonFailure();
            int quantileIndex = this->getQuantileIndex(zValues, weights, p);

            if (quantileIndex >= 0)
            {
                // perform the sampling again and recalculate
                randomSampleGenerator.restart();
                randomSampleGenerator.proceed(quantileIndex);

                std::shared_ptr<Sample> sample = randomSampleGenerator.getRandomSample();
                std::shared_ptr<Sample> modifiedSample = getModifiedSample(sample, factors, center, dimensionality);
                std::shared_ptr<Models::Evaluation> evaluation = std::make_shared<Models::Evaluation>(modelRunner->getEvaluation(modifiedSample));
                evaluation->Quantile = p;
                result.quantileEvaluations.push_back(evaluation);
            }
            else
            {
                result.quantileEvaluations.push_back(nullptr);
            }
        }

        if (this->correlationMatrixBuilder != nullptr)
        {
            this->correlationMatrixBuilder->registerWeights(weights);
            this->correlationMatrixBuilder->registerSamples(stochast, zValues);
        }

        return result;

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

    std::shared_ptr<Models::Sample> ImportanceSamplingS::getModifiedSample(const std::shared_ptr<Models::Sample> sample, std::vector<double>& factors, std::shared_ptr<Models::Sample> center, double dimensionality)
    {
        std::shared_ptr<Sample> modifiedSample = sample->clone();

        for (int k = 0; k < sample->getSize(); k++)
        {
            modifiedSample->Values[k] = factors[k] * sample->Values[k] + center->Values[k];
        }

        modifiedSample->Weight = Reliability::ImportanceSamplingSupport::getWeight(modifiedSample, sample, dimensionality);

        return modifiedSample;
    }

    void ImportanceSamplingS::updateCumulativeWeights(const std::vector<double>& zValues,
                                                      const std::vector<double>& weights,
                                                      std::vector<double>& cumulativeWeights,
                                                      const std::shared_ptr<Sample>& sample) const
    {
        for (size_t i = 0; i < cumulativeWeights.size() - 1; i++)
        {
            if (this->Settings->ProbabilityForConvergence < 0.5)
            {
                if (zValues[i] > sample->Z)
                {
                    cumulativeWeights[i] += sample->Weight;
                }
                else
                {
                    cumulativeWeights.back() += weights[i];
                }
            }
            else
            {
                if (zValues[i] < sample->Z)
                {
                    cumulativeWeights[i] += sample->Weight;
                }
                else
                {
                    cumulativeWeights.back() += weights[i];
                }
            }
        }
    }
}