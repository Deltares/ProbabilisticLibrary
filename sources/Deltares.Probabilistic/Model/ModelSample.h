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
#pragma once

#include <cmath>
#include <functional>
#include <vector>
#include <memory>

namespace Deltares
{
    namespace Models
    {
        class ModelSample
        {
        public:
            ModelSample(std::vector<double> values)
            {
                this->Values = values;
            }

            /**
             * \brief Resets all contents of the sample to its default values
             * \remark Values are not cleared, since they are provided in the constructor
             */
            void clear()
            {
                IterationIndex = -1;
                threadId = 0;
                Weight = std::nan("");
                AllowProxy = true;
                UsedProxy = false;
                IsRestartRequired = false;
                Z = std::nan("");
                Beta = 0;
                Tag = 0;
            }

            /**
             * \brief Copies the results from another sample
             */
            void copyFrom(const std::shared_ptr<ModelSample>& source)
            {
                this->Z = source->Z;
                this->OutputValues = std::vector<double>(source->OutputValues.size());
                for (size_t i = 0; i < this->OutputValues.size(); i++)
                {
                    this->OutputValues[i] = source->OutputValues[i];
                }
            }

            std::vector<double> Values;
            std::vector<double> OutputValues;

            int IterationIndex = -1;
            int threadId = 0;
            double Weight = 1;
            bool AllowProxy = true;
            bool UsedProxy = false;
            bool IsRestartRequired = false;
            double Beta = 0;
            double Z = nan("");

            bool ExtendedLogging = false;

            /**
             * Indication of a logging message
             */
            int LoggingCounter = 0;

            int Tag = 0;

            bool hasSameValues(std::shared_ptr<ModelSample> other)
            {
                if (this->Values.size() != other->Values.size())
                {
                    return false;
                }

                for (int i = 0; i < this->Values.size(); i++)
                {
                    if (this->Values[i] != other->Values[i])
                    {
                        return false;
                    }
                }

                return true;
            }

            /**
             * \brief Performs an operation on a sample resulting in a numeric value for a collection of samples
             * \param samples Collection of samples
             * \param function Operation on a sample
             * \return Resulting numeric values
             */
            template <std::predicate<std::shared_ptr<ModelSample> const&> SampleFunction>
            static std::vector<double> select(const std::vector<std::shared_ptr<ModelSample>>& samples, const SampleFunction& function)
            {
                std::vector<double> result(samples.size());

                for (size_t i = 0; i < samples.size(); i++)
                {
                    result[i] = function(samples[i]);
                }

                return result;
            }
        };
    }
}

