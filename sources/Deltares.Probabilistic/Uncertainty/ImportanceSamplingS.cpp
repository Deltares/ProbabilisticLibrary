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
#include "../Reliability/ImportanceSamplingSupport.h"

using namespace Deltares::Models;
using namespace Deltares::Reliability;

namespace Deltares::Uncertainty
{
    UncertaintyResult ImportanceSamplingS::getUncertaintyStochast(std::shared_ptr<ModelRunner> modelRunner)
    {
        modelRunner->updateStochastSettings(this->Settings->StochastSet);

        std::shared_ptr<SampleProvider> sampleProvider = std::make_shared<SampleProvider>(*Settings->StochastSet);
        modelRunner->setSampleProvider(sampleProvider);

        auto randomSampleGenerator = RandomSampleGenerator();
        randomSampleGenerator.Settings = this->Settings->randomSettings;
        randomSampleGenerator.Settings->StochastSet = this->Settings->StochastSet;
        randomSampleGenerator.sampleProvider = sampleProvider;
        randomSampleGenerator.initialize();

        int nParameters = modelRunner->getVaryingStochastCount();

        // for convergence, simulate that this is a reliability calculation with failure at given probability
        designPointBuilder = DesignPointBuilder(nParameters, DesignPointMethod::CenterOfGravity, Settings->StochastSet);

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

        std::shared_ptr<Sample> center = Settings->StochastSet->getStartPoint();

        factors = getFactors(*Settings->StochastSet);
        dimensionality = ImportanceSamplingSupport::getDimensionality(factors);

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
                    std::shared_ptr<Sample> modifiedSample = getModifiedSample(*sample, *center);
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

            updateCumulativeWeights(zValues, weights, cumulativeWeights, *sample);

            registerSample(*modelRunner, sample);

            // check whether all added samples in design point builder are supposed to be there
            // the design point builder should contain samples corresponding with a given probability of failure
            for (size_t i = 0; i < cumulativeWeights.size(); i++)
            {
                const bool shouldBeDesignPointSample = cumulativeWeights[i] <
                    sumWeights * Settings->normalizedProbabilityForConvergence();
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

            converged = getConverged(sampleIndex, *center, nSamples);
        }

        adjustWeights(weights, nSamples - sumWeights);

        std::shared_ptr<Statistics::Stochast> stochast = getStochastFromSamples(zValues, weights);

        auto result = modelRunner->getUncertaintyResult(stochast);

        for (const auto& quantile : Settings->RequestedQuantiles)
        {
            const double p = quantile->getProbabilityOfNonFailure();
            const int quantile_index = getQuantileIndex(zValues, weights, p);

            if (quantile_index >= 0)
            {
                // perform the sampling again and recalculate
                randomSampleGenerator.restart();
                randomSampleGenerator.proceed(quantile_index);

                std::shared_ptr<Sample> sample = randomSampleGenerator.getRandomSample();
                std::shared_ptr<Sample> modifiedSample = getModifiedSample(*sample, *center);
                auto evaluation = std::make_shared<Evaluation>(modelRunner->getEvaluation(modifiedSample));
                evaluation->Quantile = p;
                result.quantileEvaluations.push_back(evaluation);
            }
            else
            {
                result.quantileEvaluations.push_back(nullptr);
            }
        }

        registerWeights(weights);
        registerSamples(stochast, zValues);

        return result;
    }

    /// <summary>
    /// adjust for weights not equal to number of samples,
    /// only adjust samples with weight > 1, because they will not be in the tail of the distribution, which is the less interesting part
    /// </summary>
    /// <param name="weights"> the weights to update </param>
    /// <param name="weight_difference"> difference in weight </param>
    void ImportanceSamplingS::adjustWeights(std::vector<double>& weights, const double weight_difference)
    {
        double over_weighted_sum = 0.0;

        // calculate over weight for samples with weight > 1
        for (double weight : weights)
        {
            if (weight > 1.0)
            {
                over_weighted_sum += weight;
            }
        }

        // correct samples with weight > 1 so that sum of weights equals number of samples
        for (double& weight : weights)
        {
            if (weight > 1.0)
            {
                weight += weight * weight_difference / over_weighted_sum;
            }
        }
    }

    void ImportanceSamplingS::registerSample(const ModelRunner& modelRunner, const std::shared_ptr<Sample>& sample) const
    {
        const bool registerSamplesForCorrelation = correlationMatrixBuilder->isEmpty() &&
            Settings->CalculateCorrelations &&
            Settings->CalculateInputCorrelations;

        if (registerSamplesForCorrelation)
        {
            modelRunner.registerSample(correlationMatrixBuilder, sample);
        }
    }

    // check if convergence is reached (or stop criterion)
    bool ImportanceSamplingS::getConverged(int sampleIndex, const Sample& center, int nSamples)
    {
        bool converged = false;

        if (sampleIndex >= Settings->MinimumSamples)
        {
            const auto designPoint = designPointBuilder.getSample();

            const double designPointWeight =
                ImportanceSamplingSupport::getSampleWeight(*designPoint, center, dimensionality, factors);

            const double convergence = ImportanceSamplingSupport::getConvergence(
                 Settings->normalizedProbabilityForConvergence(), designPointWeight, nSamples);
            converged = convergence < Settings->VariationCoefficient;
        }
        return converged;
    }

    void ImportanceSamplingS::registerWeights(const std::vector<double>& weights) const
    {
        if (correlationMatrixBuilder != nullptr)
        {
            correlationMatrixBuilder->registerWeights(weights);
        }
    }

    void ImportanceSamplingS::registerSamples(const std::shared_ptr<Statistics::Stochast>& stochast,
        const std::vector<double>& zValues) const
    {
        if (correlationMatrixBuilder != nullptr)
        {
            correlationMatrixBuilder->registerSamples(stochast, zValues);
        }
    }

    std::vector<double> ImportanceSamplingS::getFactors(const StochastSettingsSet& stochastSettings)
    {
        std::vector<double> factors(stochastSettings.getVaryingStochastCount());

        for (size_t k = 0; k < factors.size(); k++)
        {
            factors[k] = stochastSettings.VaryingStochastSettings[k]->VarianceFactor;
        }

        return factors;
    }

    std::shared_ptr<Sample> ImportanceSamplingS::getModifiedSample(const Sample& sample, const Sample& center) const
    {
        std::shared_ptr<Sample> modifiedSample = sample.clone();

        for (int k = 0; k < sample.getSize(); k++)
        {
            modifiedSample->Values[k] = factors[k] * sample.Values[k] + center.Values[k];
        }

        modifiedSample->Weight = ImportanceSamplingSupport::getWeight(*modifiedSample, sample, dimensionality);

        return modifiedSample;
    }

    void ImportanceSamplingS::updateCumulativeWeights(const std::vector<double>& zValues,
                                                      const std::vector<double>& weights,
                                                      std::vector<double>& cumulativeWeights,
                                                      const Sample& sample) const
    {
        for (size_t i = 0; i < cumulativeWeights.size() - 1; i++)
        {
            if (this->Settings->ProbabilityForConvergence < 0.5)
            {
                if (zValues[i] > sample.Z)
                {
                    cumulativeWeights[i] += sample.Weight;
                }
                else
                {
                    cumulativeWeights.back() += weights[i];
                }
            }
            else
            {
                if (zValues[i] < sample.Z)
                {
                    cumulativeWeights[i] += sample.Weight;
                }
                else
                {
                    cumulativeWeights.back() += weights[i];
                }
            }
        }
    }
}