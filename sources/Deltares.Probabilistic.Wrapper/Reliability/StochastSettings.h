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

#include "../Statistics/Stochast.h"
#include "../../Deltares.Probabilistic/Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public ref class StochastSettings
            {
            private:
                SharedPointerProvider<Deltares::Reliability::StochastSettings>* shared = new SharedPointerProvider(new Deltares::Reliability::StochastSettings());

            public:
                StochastSettings() {}
                ~StochastSettings() { this->!StochastSettings(); }
                !StochastSettings() { delete shared; }

                property double UMin
                {
                    double get() { return shared->object->MinValue; }
                    void set(double value) { shared->object->MinValue = value; }
                }

                property double UMax
                {
                    double get() { return shared->object->MaxValue; }
                    void set(double value) { shared->object->MaxValue = value; }
                }

                property double StartValue
                {
                    double get() { return shared->object->StartValue; }
                    void set(double value) { shared->object->StartValue = value; }
                }

                property double VarianceFactor
                {
                    double get() { return shared->object->VarianceFactor; }
                    void set(double value) { shared->object->VarianceFactor = value; }
                }

                property bool IsVarianceAllowed
                {
                    bool get() { return shared->object->IsVarianceAllowed; }
                    void set(bool value) { shared->object->IsVarianceAllowed = value; }
                }

                property int Intervals
                {
                    int get() { return shared->object->Intervals; }
                    void set(int value) { shared->object->Intervals = value; }
                }

                property bool IsInitializationAllowed
                {
                    bool get() { return shared->object->IsInitializationAllowed; }
                    void set(bool value) { shared->object->IsInitializationAllowed = value; }
                }

                Statistics::Wrappers::Stochast^ Stochast = nullptr;

                std::shared_ptr<Reliability::StochastSettings> GetSettings()
                {
                    shared->object->stochast = this->Stochast != nullptr ? this->Stochast->GetStochast() : nullptr;
                    return shared->object;
                }
            };

            public interface class IHasStochastSetting
            {
                StochastSettings^ GetStochastSetting(Statistics::Wrappers::Stochast^ stochast);
            };
        }
    }
}


