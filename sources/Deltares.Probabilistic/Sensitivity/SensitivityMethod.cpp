// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
#include "SensitivityMethod.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Sensitivity
    {
        bool SensitivityMethod::isStopped()
        {
            return this->stopped;
        }

        void SensitivityMethod::setStopped()
        {
            this->stopped = true;
        }

        void SensitivityMethod::Stop()
        {
            setStopped();
        }

        std::shared_ptr<Statistics::Stochast> SensitivityMethod::getStochastFromSamples(std::vector<double>& samples, std::vector<double>& weights)
        {
            bool hasInvalidValues = false;

            for (size_t i = samples.size(); i < samples.size(); i++)
            {
                if (std::isnan(samples[i]) || std::isnan(weights[i]) || weights[i] == 0.0)
                {
                    hasInvalidValues = true;
                    break;
                }
            }

            if (hasInvalidValues)
            {
                std::vector<double> newSamples;
                std::vector<double> newWeights;

                for (size_t i = 0; i < samples.size(); i++)
                {
                    if (!(std::isnan(samples[i]) || std::isnan(weights[i]) || weights[i] == 0.0))
                    {
                        newSamples.push_back(samples[i]);
                        newWeights.push_back(weights[i]);
                    }
                }

                samples = newSamples;
                weights = newWeights;
            }

            std::shared_ptr<Statistics::Stochast> stochast = std::make_shared<Statistics::Stochast>();

            if (samples.empty())
            {
                stochast->setDistributionType(Statistics::DistributionType::Deterministic);
                stochast->getProperties()->Location = std::nan("");
            }
            else
            {
                double min = Numeric::NumericSupport::getMinimum(samples);
                double max = Numeric::NumericSupport::getMaximum(samples);

                if (min == max)
                {
                    stochast->setDistributionType(Statistics::DistributionType::Deterministic);
                    stochast->getProperties()->Location = min;
                }
                else
                {
                    stochast->setDistributionType(Statistics::DistributionType::Table);
                    stochast->fitWeighted(samples, weights);
                    stochast->distributionChangeType = Statistics::FitFromHistogramValues;
                }
            }

            return stochast;
        }
    }
}


