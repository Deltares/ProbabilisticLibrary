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
#pragma once

#include <vector>
#include <memory>

#include "Sample.h"
#include "ModelSample.h"
#include "../Reliability/StochastSettingsSet.h"

namespace Deltares
{
    namespace Models
    {
        class SampleProvider
        {
        public:
            SampleProvider(int varyingStochastCount, int stochastCount, bool reuseSamples)
            {
                this->sampleSize = varyingStochastCount;
                this->modelSampleSize = stochastCount;
                this->reuseSamples = reuseSamples;
            }

            SampleProvider(std::shared_ptr<Reliability::StochastSettingsSet> stochastSettings, bool reuseSamples)
            {
                this->sampleSize = stochastSettings->getVaryingStochastCount();
                this->modelSampleSize = stochastSettings->getStochastCount();
                this->reuseSamples = reuseSamples;
            }

            std::shared_ptr<Sample> getSample()
            {
                if (!reuseSamples)
                {
                    return std::make_shared<Sample>(sampleSize);
                }
                else if (nextSampleIndex < samples.size())
                {
                    std::shared_ptr<Sample> sample = samples[nextSampleIndex++];
                    sample->clear();
                    return sample;
                }
                else
                {
                    nextSampleIndex++;
                    std::shared_ptr<Sample> sample = std::make_shared<Sample>(sampleSize);
                    samples.push_back(sample);
                    return sample;
                }
            }

            std::shared_ptr<ModelSample> getModelSample(std::vector<double>& values)
            {
                if (!reuseSamples)
                {
                    return std::make_shared<ModelSample>(values);
                }
                else if (nextModelSampleIndex < modelSamples.size())
                {
                    std::shared_ptr<ModelSample> sample = modelSamples[nextModelSampleIndex++];
                    sample->clear();
                    sample->Values = values;
                    return sample;
                }
                else
                {
                    nextModelSampleIndex++;
                    std::shared_ptr<ModelSample> sample = std::make_shared<ModelSample>(values);
                    modelSamples.push_back(sample);
                    return sample;
                }
            }

            void reset()
            {
                nextSampleIndex = 0;
                nextModelSampleIndex = 0;
            }

        private:
            std::vector < std::shared_ptr<Sample>> samples;
            std::vector < std::shared_ptr<ModelSample>> modelSamples;

            int nextSampleIndex = 0;
            int nextModelSampleIndex = 0;
            bool reuseSamples = false;

            int sampleSize;
            int modelSampleSize;
        };
    }
}

