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

#include "../../Deltares.Probabilistic/Sensitivity/SingleVariationSettings.h"
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

            public ref class SingleVariationSettings : IHasRunSettings
            {
            private:
                SharedPointerProvider<Sensitivity::SingleVariationSettings>* shared = new SharedPointerProvider(new Sensitivity::SingleVariationSettings());
                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
            public:
                SingleVariationSettings() {}
                ~SingleVariationSettings() { this->!SingleVariationSettings(); }
                !SingleVariationSettings() { delete shared; }

                property double LowValue
                {
                    double get() { return shared->object->LowValue; }
                    void set(double value) { shared->object->LowValue = value; }
                }

                property double HighValue
                {
                    double get() { return shared->object->HighValue; }
                    void set(double value) { shared->object->HighValue = value; }
                }

                virtual property Wrappers::RunSettings^ RunSettings
                {
                    Wrappers::RunSettings^ get() { return runSettings; }
                    void set(Wrappers::RunSettings^ value) { runSettings = value; }
                }

                std::shared_ptr<Sensitivity::SingleVariationSettings> GetSettings()
                {
                    shared->object->RunSettings = RunSettings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}


