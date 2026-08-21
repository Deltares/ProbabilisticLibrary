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

#include <vector>

#include "ModelSample.h"

namespace Deltares::Models
{
    /**
     * \brief Keeps model samples alive
     */

    class ModelSampleStorage
    {
    private:
        std::vector<ModelSample> samples;

    public:
        /**
         * \brief Creates a registration to keep model samples alive
         * \param size The maximum number of model samples to be kept alive
         */
        ModelSampleStorage(size_t size)
        {
            samples.reserve(size);
        }

        /**
         * \brief Keeps a sample alive and returns a pointer to a model sample
         * \param sample Model sample
         * \return Pointer to model sample
         */
        ModelSample* keep(const ModelSample& sample)
        {
            samples.push_back(sample);
            return &samples.back();
        }

        /**
         * \brief Stops keeping the already registered model samples alive
         */
        void clear()
        {
            samples.clear();
        }
    };
}

