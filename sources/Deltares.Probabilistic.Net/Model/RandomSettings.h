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

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            public enum class RandomGeneratorType { MersenneTwister };

            public ref class RandomSettings
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Models::RandomSettings>* shared = new Utils::Wrappers::SharedPointerProvider(new Models::RandomSettings());
            public:
                RandomSettings()
                {
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


