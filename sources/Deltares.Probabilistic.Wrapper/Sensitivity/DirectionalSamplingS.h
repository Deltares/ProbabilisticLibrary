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

#include "DirectionalSamplingSettingsS.h"
#include "SensitivityMethod.h"
#include "../../Deltares.Probabilistic/Sensitivity/DirectionalSamplingS.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            public ref class DirectionalSamplingS : public SensitivityMethod
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Sensitivity::DirectionalSamplingS>* shared = new Utils::Wrappers::SharedPointerProvider(new Sensitivity::DirectionalSamplingS());
            public:
                DirectionalSamplingS() { }
                ~DirectionalSamplingS() { this->!DirectionalSamplingS(); }
                !DirectionalSamplingS() { delete shared; }

                DirectionalSamplingSettingsS^ Settings = gcnew DirectionalSamplingSettingsS();

                System::Object^ GetSettings() override { return Settings; }

                bool IsValid() override { return Settings->IsValid(); }

                std::shared_ptr<Sensitivity::SensitivityMethod> GetNativeSensitivityMethod() override
                {
                    shared->object->Settings = Settings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}



