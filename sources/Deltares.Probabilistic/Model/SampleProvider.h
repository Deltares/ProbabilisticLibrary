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
            SampleProvider(int varyingStochastCount, int stochastCount)
            {
                this->sampleSize = varyingStochastCount;
                this->modelSampleSize = stochastCount;
            }

            SampleProvider(std::shared_ptr<Reliability::StochastSettingsSet> stochastSettings)
            {
                this->sampleSize = stochastSettings->getVaryingStochastCount();
                this->modelSampleSize = stochastSettings->getStochastCount();
            }

            std::shared_ptr<Sample> getSample()
            {
                return std::make_shared<Sample>(sampleSize);
            }

            std::shared_ptr<ModelSample> getModelSample(std::vector<double>& values)
            {
                return std::make_shared<ModelSample>(values);
            }

        private:
            int sampleSize;
            int modelSampleSize;
        };
    }
}

