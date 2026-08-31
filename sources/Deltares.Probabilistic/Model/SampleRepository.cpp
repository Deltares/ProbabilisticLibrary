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
    void SampleRepository::SampleCollection::registerSample(ModelSample& sample)
    {
        this->samples.push_back(sample.clone());
    }

    bool SampleRepository::SampleCollection::retrieveSample(ModelSample& sample) const
    {
        for (auto existingSample : samples)
        {
            if (existingSample.hasSameValues(sample))
            {
                sample.copyFrom(existingSample);
                return true;
            }
        }

        return false;
    }

    double SampleRepository::getKey(ModelSample& sample)
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

    bool SampleRepository::shouldRegisterSample(ModelSample& sample)
    {
        return !sample.UsedProxy;
    }

    void SampleRepository::registerSample(ModelSample& sample)
    {
        if (shouldRegisterSample(sample))
        {
            double key = this->getKey(sample);

            if (!std::isnan(key) && !std::isinf(key))
            {
                locker->lock();

                if (!this->sampleCollections.contains(key))
                {
                    sampleCollections[key] = std::make_unique<SampleCollection>();
                }

                locker->unlock();

                sampleCollections[key]->registerSample(sample);
            }
        }
    }

    bool SampleRepository::retrieveSample(ModelSample& sample)
    {
        double key = this->getKey(sample);

        if (!this->sampleCollections.contains(key))
        {
            return false;
        }

        return this->sampleCollections[key]->retrieveSample(sample);
    }

    void SampleRepository::clear()
    {
        this->sampleCollections.clear();
    }
}

