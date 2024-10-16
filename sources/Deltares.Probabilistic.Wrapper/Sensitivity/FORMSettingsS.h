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

#include "../../Deltares.Probabilistic/Sensitivity/FORMSettingsS.h"
#include "../Model/RunSettings.h"
#include "../Model/GradientSettings.h"
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

            public ref class FORMSettingsS : IHasRunSettings
            {
            private:
                SharedPointerProvider<Sensitivity::FORMSettingsS>* shared = new SharedPointerProvider(new Sensitivity::FORMSettingsS());
                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
            public:
                FORMSettingsS() {}
                ~FORMSettingsS() { this->!FORMSettingsS(); }
                !FORMSettingsS() { delete shared; }

                property double Minimum
                {
                    double get() { return shared->object->Minimum; }
                    void set(double value) { shared->object->Minimum = value; }
                }

                property double Maximum
                {
                    double get() { return shared->object->Maximum; }
                    void set(double value) { shared->object->Maximum = value; }
                }

                property double StepSize
                {
                    double get() { return shared->object->StepSize; }
                    void set(double value) { shared->object->StepSize = value; }
                }

                /**
                 * \brief Settings for calculating the gradient at a stochast point
                 */
                Wrappers::GradientSettings^ GradientSettings = gcnew Wrappers::GradientSettings();

                virtual property Wrappers::RunSettings^ RunSettings
                {
                    Wrappers::RunSettings^ get() { return runSettings; }
                    void set(Wrappers::RunSettings^ value) { runSettings = value; }
                }

                bool IsValid()
                {
                    return shared->object->isValid();
                }

                std::shared_ptr<Sensitivity::FORMSettingsS> GetSettings()
                {
                    shared->object->GradientSettings = this->GradientSettings->GetSettings();
                    shared->object->RunSettings = RunSettings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}


