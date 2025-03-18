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

#include <memory>
#include <unordered_map>

#include "ModelSample.h"

namespace Deltares::Models
{
    /**
     * \brief Stores samples for reuse
     */
    class SampleRepository
    {
    public:
        SampleRepository()
        {
        }

        void registerSample(std::shared_ptr<ModelSample> sample);

        std::shared_ptr<ModelSample> retrieveSample(std::shared_ptr<ModelSample> sample);

    private:
        class SampleCollection
        {
        public:
            void registerSample(std::shared_ptr<ModelSample> sample);
            std::shared_ptr<ModelSample> retrieveSample(std::shared_ptr<ModelSample> sample) const;
            int size() { return static_cast<int>(samples.size()); }
        private:
            std::vector<std::shared_ptr<ModelSample>> samples;
        };

        std::unordered_map<double, std::unique_ptr<SampleCollection>> sampleCollections;
        double getKey(std::shared_ptr<ModelSample> sample);
    };
}

