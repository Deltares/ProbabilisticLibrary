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

#include "../../Deltares.Probabilistic/Uncertainty/FOSMSettings.h"
#include "../Model/RunSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Uncertainty
    {
        namespace Wrappers
        {
            public ref class FOSMSettings : Models::Wrappers::IHasRunSettings
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Uncertainty::FOSMSettings>* shared = new Utils::Wrappers::SharedPointerProvider(new Uncertainty::FOSMSettings());
                Models::Wrappers::RunSettings^ runSettings = gcnew Models::Wrappers::RunSettings();
            public:
                FOSMSettings() {}
                ~FOSMSettings() { this->!FOSMSettings(); }
                !FOSMSettings() { delete shared; }

                property bool CalculateCorrelations
                {
                    bool get() { return shared->object->CalculateCorrelations; }
                    void set(bool value) { shared->object->CalculateCorrelations = value; }
                }

                property bool CalculateInputCorrelations
                {
                    bool get() { return shared->object->CalculateInputCorrelations; }
                    void set(bool value) { shared->object->CalculateInputCorrelations = value; }
                }

                property double StepSize
                {
                    double get() { return shared->object->StepSize; }
                    void set(double value) { shared->object->StepSize = value; }
                }


                virtual property Models::Wrappers::RunSettings^ RunSettings
                {
                    Models::Wrappers::RunSettings^ get() { return runSettings; }
                    void set(Models::Wrappers::RunSettings^ value) { runSettings = value; }
                }

                std::shared_ptr<Uncertainty::FOSMSettings> GetSettings()
                {
                    shared->object->RunSettings = RunSettings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}


