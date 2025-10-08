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

#include "FOSMSettings.h"
#include "UncertaintyMethod.h"
#include "../../Deltares.Probabilistic/Uncertainty/FOSM.h"

namespace Deltares
{
    namespace Uncertainty
    {
        namespace Wrappers
        {
            public ref class FOSM : public UncertaintyMethod
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Uncertainty::FOSM>* shared = new Utils::Wrappers::SharedPointerProvider(new Uncertainty::FOSM());
            public:
                FOSM() { }
                ~FOSM() { this->!FOSM(); }
                !FOSM() { delete shared; }

                FOSMSettings^ Settings = gcnew FOSMSettings();

                System::Object^ GetSettings() override { return Settings; }

                std::shared_ptr<Uncertainty::UncertaintyMethod> GetNativeSensitivityMethod() override
                {
                    shared->object->Settings = Settings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}



