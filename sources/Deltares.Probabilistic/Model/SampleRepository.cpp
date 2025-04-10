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

#include "SampleRepository.h"

namespace Deltares::Models
{
    void SampleRepository::SampleCollection::registerSample(const std::shared_ptr<ModelSample>& sample)
    {
        samples.push_back(sample);
    }

    std::shared_ptr<ModelSample> SampleRepository::SampleCollection::retrieveSample(const ModelSample& sample) const
    {
        for (const auto& existingSample : samples)
        {
            if (existingSample->hasSameValues(sample))
            {
                return existingSample;
            }
        }

        return nullptr;
    }

    double SampleRepository::getKey(const ModelSample& sample)
    {
        // calculates a sample key for the sample sample
        double sum = 0;
        for (size_t i = 0; i < sample.Values.size(); i++)
        {
            int index = static_cast<int>(i) + 1;
            sum += index * sample.Values[i];
        }

        return sum;
    }

    void SampleRepository::registerSample(const std::shared_ptr<ModelSample>& sample)
    {
        double key = getKey(*sample);

        if (!sampleCollections.contains(key))
        {
            sampleCollections[key] = SampleCollection();
        }

        sampleCollections[key].registerSample(sample);
    }

    std::shared_ptr<ModelSample> SampleRepository::retrieveSample(const ModelSample& sample)
    {
        double key = getKey(sample);

        if (!sampleCollections.contains(key))
        {
            return nullptr;
        }

        return sampleCollections[key].retrieveSample(sample);
    }
}

