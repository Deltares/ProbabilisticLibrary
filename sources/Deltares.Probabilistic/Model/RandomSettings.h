#pragma once

#include "../Math/Random.h"
#include "../Reliability/StochastSettingsSet.h"

namespace Deltares
{
    namespace Models
    {
        /**
         * \brief Settings for generating random values
         */
        class RandomSettings
        {
        public:
            /**
             * \brief Indicates whether each time the same random values are generated
             */
            bool IsRepeatableRandom = true;

            /**
             * \brief Indicates whether random sequences is always the same for same <see cref="IHasStochast"/> names
             */
            bool IsStochastRepeatableRandom = false;

            /**
             * \brief Type of random generator algorithm
             */
            Deltares::Numeric::RandomValueGeneratorType RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::ModifiedKnuthSubtractive;

            /**
             * \brief Seed for random generator
             */
            int Seed = 0;

            /**
             * \brief Additional seed for random generator
             */
            int SeedB = 0;

            /**
             * \brief Indicates whether a dummy random value is generated for non varying variables (true) or not (false)
             * \remark Skipping unvarying parameters is useful when changing a variable from varying to non varying. Then the same random values are generated for other (varying) variables.
             */
            bool SkipUnvaryingParameters = true;

            std::shared_ptr<Deltares::Reliability::StochastSettingsSet> StochastSet = std::make_shared<Deltares::Reliability::StochastSettingsSet>();

            /**
             * \brief Gets a copy of these settings
             * \return Copy
             */
            std::shared_ptr<RandomSettings> clone()
            {
                std::shared_ptr<RandomSettings> copy = std::make_shared<RandomSettings>();

                copy->IsRepeatableRandom = this->IsRepeatableRandom;
                copy->IsStochastRepeatableRandom = this->IsStochastRepeatableRandom;
                copy->RandomGeneratorType = this->RandomGeneratorType;
                copy->Seed = this->Seed;
                copy->SeedB = this->SeedB;
                copy->StochastSet = this->StochastSet;

                return copy;
            }
        };
    }
}

