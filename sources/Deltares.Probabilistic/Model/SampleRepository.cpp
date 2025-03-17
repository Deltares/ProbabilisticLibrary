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

#include "SampleRepository.h"

namespace Deltares::Models
{
    double SampleRepository::getKey(std::shared_ptr<ModelSample> sample)
    {
        // calculate value from sample
        double sum = 0;
        for (size_t i = 0; i < sample->Values.size(); i++)
        {
            int index = static_cast<int>(i) + 1;
            sum += index * sample->Values[i];
        }

        return sum;
    }

    void SampleRepository::registerSample(std::shared_ptr<ModelSample> sample)
    {
        double key = this->getKey(sample);

        if (!this->samples.contains(key))
        {
            std::vector<std::shared_ptr<ModelSample>> newCollection;
            samples[key] = newCollection;
        }

        std::vector<std::shared_ptr<ModelSample>> sampleCollection = samples[key];
        sampleCollection.push_back(sample);
    }

    std::shared_ptr<ModelSample> SampleRepository::retrieveSample(std::shared_ptr<ModelSample> sample)
    {
        double key = this->getKey(sample);

        if (!this->samples.contains(key))
        {
            return nullptr;
        }

        std::vector<std::shared_ptr<ModelSample>> sampleCollection = samples[key];

        for (std::shared_ptr<ModelSample> existingSample : sampleCollection)
        {
            if (existingSample->hasSameValues(sample))
            {
                return existingSample;
            }
        }

        return nullptr;
    }
}

