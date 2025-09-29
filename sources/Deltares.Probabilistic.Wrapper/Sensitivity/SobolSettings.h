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

#include "../../Deltares.Probabilistic/Sensitivity/SobolSettings.h"
#include "../Model/RunSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class SobolSettings : IHasRunSettings
            {
            private:
                SharedPointerProvider<Sensitivity::SobolSettings>* shared = new SharedPointerProvider(new Sensitivity::SobolSettings());
                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
            public:
                SobolSettings() {}
                ~SobolSettings() { this->!SobolSettings(); }
                !SobolSettings() { delete shared; }

                property int Iterations
                {
                    int get() { return shared->object->Iterations; }
                    void set(int value) { shared->object->Iterations = value; }
                }

                virtual property Wrappers::RunSettings^ RunSettings
                {
                    Wrappers::RunSettings^ get() { return runSettings; }
                    void set(Wrappers::RunSettings^ value) { runSettings = value; }
                }

                std::shared_ptr<Sensitivity::SobolSettings> GetSettings()
                {
                    shared->object->RunSettings = RunSettings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}


