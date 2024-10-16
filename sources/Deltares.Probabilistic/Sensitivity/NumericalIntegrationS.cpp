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
#include "NumericalIntegrationS.h"
#include <vector>
#include <cmath>
#include <memory>

#include "../Math/NumericSupport.h"
#include "../Model/Sample.h"
#include "../Model/GradientCalculator.h"
#include "../Statistics/DistributionType.h"

using namespace Deltares::Models;

namespace Deltares
{
    namespace Sensitivity
    {
        std::shared_ptr<Statistics::Stochast> NumericalIntegrationS::getStochast(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(Settings->StochastSet);

            int nStochasts = modelRunner->getVaryingStochastCount();

            // Numerical integration isn't possible with a large set of stochastic parameters
            // warnings and errors are presented.
            if (nStochasts > 11)
            {
                modelRunner->reportMessage(MessageType::Error, "Numerical integration with more than 11 stochastic parameters. This is practically impossible.");
                return nullptr;
            }
            else if (nStochasts > 4)
            {
                modelRunner->reportMessage(MessageType::Warning, "Numerical integration with more than 4 stochastic parameters. Large computation time is possible.");
            }

            std::vector<double> zSamples;

            // Initialize parameters for the recursive part of the numerical integration computation.
            constexpr double density = 1.0; // joint probability density
            constexpr int stochastIndex = 0; // number of the stochastic parameter
            std::shared_ptr<Sample> rootSample = std::make_shared<Sample>(nStochasts); //local vector with values in u-space
            bool registerSamplesForCorrelation = this->correlationMatrixBuilder->isEmpty() && this->Settings->CalculateCorrelations && this->Settings->CalculateInputCorrelations;

            std::vector<std::shared_ptr<Numeric::WeightedValue>> samples = collectSamples(modelRunner, stochastIndex, rootSample, density, registerSamplesForCorrelation);

            std::shared_ptr<Statistics::Stochast> stochast = this->getStochastFromSamples(samples);

            if (this->Settings->CalculateCorrelations)
            {
                this->correlationMatrixBuilder->registerWeightedValues(stochast, samples);
            }

            return stochast;
        }

        std::vector<std::shared_ptr<Numeric::WeightedValue>> NumericalIntegrationS::collectSamples(std::shared_ptr<ModelRunner> modelRunner, int stochastIndex, std::shared_ptr<Sample> parentSample, double density, bool registerSamplesForCorrelation)
        {
            const double uDelta = 0.01;
            const int nStochasts = Settings->StochastSet->getVaryingStochastCount();

            // Initialize parameters for stochastic parameter u.
            const int nrIntervals = Settings->StochastSet->VaryingStochastSettings[stochastIndex]->Intervals; // number of intervals in u-space as integer
            const double uMin = Settings->StochastSet->VaryingStochastSettings[stochastIndex]->MinValue; // lower bound for u
            const double uMax = Settings->StochastSet->VaryingStochastSettings[stochastIndex]->MaxValue; // upper bound for u
            const double rangeU = uMax - uMin;

            // Build up list of u values to be computed
            std::vector<double> uValues = { -Statistics::StandardNormal::UMax };

            for (int i = 0; i <= nrIntervals; i++)
            {
                double uValue = uMin + i * rangeU / nrIntervals;
                if (uValue > -Statistics::StandardNormal::UMax + uDelta && uValue < Statistics::StandardNormal::UMax - uDelta)
                {
                    uValues.push_back(uValue);
                }
            }
            uValues.push_back(Statistics::StandardNormal::UMax);

            // Initialize first probabilities
            Statistics::PQ pq = Statistics::StandardNormal::getPQFromU(uValues[0]);

            if (stochastIndex < nStochasts - 1)
            {
                std::vector<std::shared_ptr<Numeric::WeightedValue>> values;

                for (size_t j = 0; j < uValues.size() - 1; j++)
                {
                    parentSample->Values[stochastIndex] = (uValues[j] + uValues[j + 1]) / 2;

                    const Statistics::PQ pqPrev = pq;
                    pq = Statistics::StandardNormal::getPQFromU(uValues[j + 1]);

                    // depending on the value of u(i) use the probabilities of exceeding or the probabilities of non-exceeding
                    const double contribution = parentSample->Values[stochastIndex] < 0 ? pq.p - pqPrev.p : pqPrev.q - pq.q;

                    std::vector<std::shared_ptr<Numeric::WeightedValue>> newValues = collectSamples(modelRunner, stochastIndex + 1, parentSample, density * contribution, registerSamplesForCorrelation);
                    for (std::shared_ptr<Numeric::WeightedValue> v : newValues)
                    {
                        values.push_back(v);
                    }

                    if (stochastIndex == 0)
                    {
                        modelRunner->reportProgress(static_cast<int>(j), static_cast<int> (uValues.size()));
                    }
                }

                return values;
            }
            else
            {
                std::vector<std::shared_ptr<Numeric::WeightedValue>> values;

                std::vector<std::shared_ptr<Sample>> samples;

                for (size_t j = 0; j < uValues.size() - 1; j++)
                {
                    std::shared_ptr<Sample> sample = parentSample->clone();
                    sample->Values[stochastIndex] = (uValues[j] + uValues[j + 1]) / 2;

                    samples.push_back(sample);
                }

                // compute the z-value(s)
                const std::vector<double> zValues = modelRunner->getZValues(samples); 

                for (size_t j = 0; j < samples.size(); j++)
                {
                    const std::shared_ptr<Sample> sample = samples[j];

                    if (!std::isnan(sample->Z))
                    {
                        Statistics::PQ pqPrev = pq;
                        pq = Statistics::StandardNormal::getPQFromU(uValues[j + 1]);

                        // depending on the value of u(i) use the probabilities of exceeding or the probabilities of non-exceeding
                        const double contribution = sample->Values[stochastIndex] < 0 ? pq.p - pqPrev.p : pqPrev.q - pq.q;

                        sample->Weight = density * contribution;

                        values.push_back(std::make_shared<Numeric::WeightedValue>(sample->Z, sample->Weight));

                        if (registerSamplesForCorrelation)
                        {
                            modelRunner->registerSample(this->correlationMatrixBuilder, sample);
                        }
                    }
                }

                return values;
            }
        }
    }
}



