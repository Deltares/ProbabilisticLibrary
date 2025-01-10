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
#include "../../Deltares.Probabilistic/Model/RandomSettings.h"
#include "../Utils/SharedPointerProvider.h"
#include "RandomProvider.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            using namespace Deltares::Models;
            using namespace Deltares::Utils::Wrappers;

            public enum class RandomGeneratorType { MersenneTwister };

            public ref class RandomSettings
            {
            private:
                SharedPointerProvider<Models::RandomSettings>* shared = new SharedPointerProvider(new Models::RandomSettings());

                bool hasLimitedRandomValues = false;
                bool isStochastRepeatableRandom = false;

            public:
                RandomSettings()
                {
                    RandomProvider::initialize();
                }
                ~RandomSettings() { this->!RandomSettings(); }
                !RandomSettings() { delete shared; }

                property int Seed
                {
                    int get() { return shared->object->Seed; }
                    void set(int value) { shared->object->Seed = value; }
                }

                property bool IsRepeatableRandom
                {
                    bool get() { return shared->object->IsRepeatableRandom; }
                    void set(bool value) { shared->object->IsRepeatableRandom = value; }
                }

                /// <summary>
                /// Indicates whether the randomizer should draw dummy random values for unvarying parameters,
                /// so that changing distributions from varying to unvarying wil not affect the random values of varying parameters
                /// </summary>
                property bool SkipUnvaryingParameters
                {
                    bool get() { return shared->object->SkipUnvaryingParameters; }
                    void set(bool value) { shared->object->SkipUnvaryingParameters = value; }
                }

                property RandomGeneratorType RandomGeneratorType
                {
                    Wrappers::RandomGeneratorType get()
                    {
                        return Wrappers::RandomGeneratorType::MersenneTwister;
                    }
                    void set(Wrappers::RandomGeneratorType value)
                    {
                        shared->object->RandomGeneratorType = Numeric::RandomValueGeneratorType::MersenneTwister;
                    }
                }

                // TODO: PROBL-42 remove this property when RandomValueGenerator is not used any more

                /// <summary>
                /// Indicates whether random values a limited to a certain range
                /// </summary>
                property bool HasLimitedRandomValues
                {
                    bool get() { return hasLimitedRandomValues; }
                    void set(bool value) { hasLimitedRandomValues = value; }
                }

                // TODO: PROBL-42 remove this property when RandomValueGenerator is not used any more
                /// <summary>
                /// Indicates whether random sequences is always the same for same <see cref="IHasStochast"/> names
                /// </summary>
                property bool IsStochastRepeatableRandom
                {
                    bool get() { return isStochastRepeatableRandom; }
                    void set(bool value) { isStochastRepeatableRandom = value; }
                }


                std::shared_ptr<Models::RandomSettings> GetSettings()
                {
                    return shared->object;
                }
            };

            public interface class IHasRandomSettings
            {
                property RandomSettings^ RandomSettings;
            };
        }
    }
}


