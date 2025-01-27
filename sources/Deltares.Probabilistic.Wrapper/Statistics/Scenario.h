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

#include "../../Deltares.Probabilistic/Statistics/Scenario.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Utils/NativeSupport.h"
#include "../Statistics/Stochast.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            public ref class Scenario
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Statistics::Scenario>* shared = new Utils::Wrappers::SharedPointerProvider(new Statistics::Scenario());
                Stochast^ parameter = gcnew Stochast();
            public:
                virtual property System::String^ Name
                {
                    System::String^ get() { return Utils::Wrappers::NativeSupport::toManaged(shared->object->name); }
                    void set(System::String^ value) { shared->object->name = Utils::Wrappers::NativeSupport::toNative(value); }
                }

                virtual property double Probability
                {
                    double get() { return shared->object->probability; }
                    void set(double value) { shared->object->probability = value; }
                }

                virtual property Stochast^ Parameter
                {
                    Stochast^ get() { return parameter; }
                    void set(Stochast^ value) { parameter = value; }
                }

                virtual property double PhysicalValue
                {
                    double get() { return shared->object->parameterValue; }
                    void set(double value) { shared->object->parameterValue = value; }
                }

                virtual Stochast^ GetStochast()
                {
                    return this->Parameter;
                }

                std::shared_ptr<Statistics::Scenario> GetNativeScenario()
                {
                    shared->object->parameter = this->Parameter != nullptr ? this->Parameter->GetStochast() : nullptr;
                    shared->object->probability = this->Probability;
                    shared->object->parameterValue = this->PhysicalValue;

                    Stochast^ stochast = this->GetStochast();
                    shared->object->parameter = stochast != nullptr ? stochast->GetStochast() : nullptr;

                    return shared->object;
                }
            };
        }
    }
}
