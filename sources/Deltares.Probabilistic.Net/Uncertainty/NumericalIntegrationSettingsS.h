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

#include "../../Deltares.Probabilistic/Uncertainty/NumericalIntegrationSettingsS.h"
#include "../Model/RunSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Uncertainty
    {
        namespace Wrappers
        {
            public ref class NumericalIntegrationSettingsS : Models::Wrappers::IHasRunSettings
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Uncertainty::NumericalIntegrationSettingsS>* shared = new Utils::Wrappers::SharedPointerProvider(new Uncertainty::NumericalIntegrationSettingsS());
                Models::Wrappers::RunSettings^ runSettings = gcnew Models::Wrappers::RunSettings();
            public:
                NumericalIntegrationSettingsS() {}
                ~NumericalIntegrationSettingsS() { this->!NumericalIntegrationSettingsS(); }
                !NumericalIntegrationSettingsS() { delete shared; }

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

                System::Collections::Generic::List<Reliability::Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Reliability::Wrappers::StochastSettings^>();

                virtual property Models::Wrappers::RunSettings^ RunSettings
                {
                    Models::Wrappers::RunSettings^ get() { return runSettings; }
                    void set(Models::Wrappers::RunSettings^ value) { runSettings = value; }
                }

                std::shared_ptr<Uncertainty::NumericalIntegrationSettingsS> GetSettings()
                {
                    shared->object->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < StochastSettings->Count; i++)
                    {
                        shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
                    }

                    shared->object->RunSettings = RunSettings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}


