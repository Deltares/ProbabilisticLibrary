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
#include <vector>
#include <memory>

#include "ModelSampleStruct.h"

namespace Deltares::Models
{
    class ModelSample
    {
    public:
        ModelSample(std::vector<double> values)
        {
            this->Values = values;
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

        bool hasSameValues(std::shared_ptr<ModelSample> other);

        /**
         * \brief Resets all contents of the sample to its default values
         * \remark Values are not cleared, since they are provided in the constructor
         */
        void clear();

        /**
         * \brief Copies the results from another sample
         */
        void copyFrom(const std::shared_ptr<ModelSample>& source);

        ModelSampleStruct getModelSampleStruct() const;

        void setModelSampleStruct(ModelSampleStruct* sampleStruct);

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


