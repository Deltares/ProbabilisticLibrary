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
        /**
         * \brief Initializes the random sample generator
         */
        void RandomSampleGenerator::initialize()
        {
            random.initialize(this->Settings->IsRepeatableRandom, this->Settings->Seed, this->Settings->getTimeStamp());

            if (sampleProvider == nullptr)
            {
                sampleProvider = std::make_shared<SampleProvider>(this->Settings->StochastSet);
            }
        }

        /**
         * \brief Restarts the sample generator and
         * \remarks This will enforce the random sample generator to produce equal samples, even if repeatable is false
         */
        void RandomSampleGenerator::restart()
        {
            random.restart();
        }

        /**
         * \brief Gets a random sample
         * \returns Random sample
         */
        std::shared_ptr<Sample> RandomSampleGenerator::getRandomSample()
        {
            const int size = this->getSampleSize();

            std::vector<double> randomValues = std::vector<double>(size);

            for (int i = 0; i < size; i++)
            {
                randomValues[i] = random.next();
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

        void RandomSampleGenerator::proceed(int nSamples)
        {
            const int size = getSampleSize();
            for (int i = 0; i < nSamples; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    double _ = random.next();
                }
            }
        }

        int RandomSampleGenerator::getSampleSize() const
        {
            return this->Settings->SkipUnvaryingParameters
                ? this->Settings->StochastSet->getStochastCount()
                : this->Settings->StochastSet->getVaryingStochastCount();
        }
    }
}
