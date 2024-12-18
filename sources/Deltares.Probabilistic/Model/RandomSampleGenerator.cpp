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
#include <vector>
#include "RandomSampleGenerator.h"

#include "SampleProvider.h"
#include "../Statistics/StandardNormal.h"

namespace Deltares
{
    namespace Models
    {
        void RandomSampleGenerator::initialize()
        {
            random.initialize(this->Settings->RandomGeneratorType, this->Settings->IsRepeatableRandom, this->Settings->Seed, this->Settings->SeedB);

            if (sampleProvider == nullptr)
            {
                sampleProvider = std::make_shared<SampleProvider>(this->Settings->StochastSet, false);
            }
        }

        void RandomSampleGenerator::restart()
        {
            random.restart();

            if (sampleProvider != nullptr)
            {
                sampleProvider->reset();
            }
        }

        std::shared_ptr<Sample> RandomSampleGenerator::getRandomSample()
        {
            std::vector<double> randomValues;

            const int size = this->Settings->SkipUnvaryingParameters ? this->Settings->StochastSet->getStochastCount() : this->Settings->StochastSet->getVaryingStochastCount();
            for (int i = 0; i < size; i++)
            {
                randomValues.push_back(random.next());
            }

            std::shared_ptr<Sample> sample = sampleProvider->getSample();

            for (int i = 0; i < this->Settings->StochastSet->getVaryingStochastCount(); i++)
            {
                const int variableIndex = this->Settings->SkipUnvaryingParameters ? this->Settings->StochastSet->VaryingStochastSettings[i]->StochastIndex : i;
                const double x = randomValues[variableIndex];
                sample->Values[i] = Deltares::Statistics::StandardNormal::getUFromQ(x);
            }

            return sample;
        }
    }
}
