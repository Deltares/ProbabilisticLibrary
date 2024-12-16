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

#include <memory>

#include "../../Deltares.Probabilistic/Model/GradientSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            public enum class GradientType
            {
                OneDirection,
                TwoDirections
            };

            public ref class GradientSettings
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Models::GradientSettings>* shared = new Utils::Wrappers::SharedPointerProvider<Models::GradientSettings>(new Models::GradientSettings());
            public:
                GradientSettings() {}
                ~GradientSettings() { this->!GradientSettings(); }
                !GradientSettings() { delete shared; }

                property double StepSize
                {
                    double get() { return shared->object->StepSize; }
                    void set(double value) { shared->object->StepSize = value; }
                }

                property Wrappers::GradientType GradientType
                {
                    Wrappers::GradientType get()
                    {
                        switch (shared->object->gradientType)
                        {
                        case Deltares::Models::GradientType::OneDirection: return Wrappers::GradientType::OneDirection;
                        case Deltares::Models::GradientType::TwoDirections: return Wrappers::GradientType::TwoDirections;
                        default: throw gcnew System::NotSupportedException("gradient type");
                        }
                    }
                    void set(Wrappers::GradientType value)
                    {
                        switch (value)
                        {
                        case Wrappers::GradientType::OneDirection: shared->object->gradientType = Models::GradientType::OneDirection; break;
                        case Wrappers::GradientType::TwoDirections: shared->object->gradientType = Models::GradientType::TwoDirections; break;
                        default: throw gcnew System::NotSupportedException("gradient type");
                        }
                    }
                }

                std::shared_ptr<Models::GradientSettings> GetSettings()
                {
                    return shared->object;
                }
            };
        }
    }
}



