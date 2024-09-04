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

#include "../../Deltares.Probabilistic/Reliability/StartPointCalculatorSettings.h"
#include "../../Deltares.Probabilistic/Reliability/StochastSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public enum class StartMethodType
            {
                None,
                RaySearch,
                SensitivitySearch,
                SphereSearch
            };

            public ref class StartPointCalculatorSettings
            {
            private:
                SharedPointerProvider<Reliability::StartPointCalculatorSettings>* shared = new SharedPointerProvider(new Reliability::StartPointCalculatorSettings());

            public:
                StartPointCalculatorSettings() {}
                ~StartPointCalculatorSettings() { this->!StartPointCalculatorSettings(); }
                !StartPointCalculatorSettings() { delete shared; }

                property double MaximumLengthStartPoint
                {
                    double get() { return shared->object->MaximumLengthStartPoint; }
                    void set(double value) { shared->object->MaximumLengthStartPoint = value; }
                }

                property double GradientStepSize
                {
                    double get() { return shared->object->GradientStepSize; }
                    void set(double value) { shared->object->GradientStepSize = value; }
                }

                property double RadiusSphereSearch
                {
                    double get() { return shared->object->RadiusSphereSearch; }
                    void set(double value) { shared->object->RadiusSphereSearch = value; }
                }

                property bool AllQuadrants
                {
                    bool get() { return shared->object->allQuadrants; }
                    void set(bool value) { shared->object->allQuadrants = value; }
                }

                property Wrappers::StartMethodType StartMethod
                {
                    Wrappers::StartMethodType get()
                    {
                        switch (shared->object->StartMethod)
                        {
                        case Deltares::Reliability::StartMethodType::None: return Wrappers::StartMethodType::None;
                        case Deltares::Reliability::StartMethodType::RaySearch: return Wrappers::StartMethodType::RaySearch;
                        case Deltares::Reliability::StartMethodType::SensitivitySearch: return Wrappers::StartMethodType::SensitivitySearch;
                        case Deltares::Reliability::StartMethodType::SphereSearch: return Wrappers::StartMethodType::SphereSearch;
                        default: throw gcnew System::NotSupportedException("start method");
                        }
                    }
                    void set(Wrappers::StartMethodType value)
                    {
                        switch (value)
                        {
                        case Wrappers::StartMethodType::None: shared->object->StartMethod = Reliability::None; break;
                        case Wrappers::StartMethodType::RaySearch: shared->object->StartMethod = Reliability::RaySearch; break;
                        case Wrappers::StartMethodType::SensitivitySearch: shared->object->StartMethod = Reliability::SensitivitySearch; break;
                        case Wrappers::StartMethodType::SphereSearch: shared->object->StartMethod = Reliability::SphereSearch; break;
                        default: throw gcnew System::NotSupportedException("Start method");
                        }
                    }
                }

                bool IsValid()
                {
                    return shared->object->isValid();
                }

                System::Collections::Generic::List<Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Wrappers::StochastSettings^>();

                std::shared_ptr<Reliability::StartPointCalculatorSettings> GetSettings()
                {
                    shared->object->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < StochastSettings->Count; i++)
                    {
                        shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
                    }

                    return shared->object;
                }
            };
        }
    }
}


