// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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

#include "../../Deltares.Probabilistic/Sensitivity/DirectionalSamplingSettingsS.h"
#include "../Model/RunSettings.h"
#include "../Model/RandomSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class DirectionalSamplingSettingsS : IHasRunSettings
            {
            private:
                SharedPointerProvider<Sensitivity::DirectionalSamplingSettingsS>* shared = new SharedPointerProvider(new Sensitivity::DirectionalSamplingSettingsS());
                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
            public:
                DirectionalSamplingSettingsS() {}
                ~DirectionalSamplingSettingsS() { this->!DirectionalSamplingSettingsS(); }
                !DirectionalSamplingSettingsS() { delete shared; }

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

                Wrappers::RandomSettings^ RandomSettings = gcnew Wrappers::RandomSettings();

                virtual property Wrappers::RunSettings^ RunSettings
                {
                    Wrappers::RunSettings^ get() { return runSettings; }
                    void set(Wrappers::RunSettings^ value) { runSettings = value; }
                }

                bool IsValid()
                {
                    return shared->object->isValid();
                }

                std::shared_ptr<Sensitivity::DirectionalSamplingSettingsS> GetSettings()
                {
                    shared->object->randomSettings = RandomSettings->GetSettings(),
                    shared->object->RunSettings = RunSettings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}


