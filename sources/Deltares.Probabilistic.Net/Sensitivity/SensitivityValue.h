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

#include "../../Deltares.Probabilistic/Sensitivity/SensitivityResult.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Statistics/Stochast.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            public ref class SensitivityValue
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Sensitivity::SensitivityValue>* shared = nullptr;
            public:
                SensitivityValue()
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(new Deltares::Sensitivity::SensitivityValue());
                }

                SensitivityValue(std::shared_ptr<Deltares::Sensitivity::SensitivityValue> sensitivityValue)
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(sensitivityValue);
                }
                ~SensitivityValue() { this->!SensitivityValue(); }
                !SensitivityValue() { delete shared; }

                Statistics::Wrappers::Stochast^ Parameter = nullptr;

                property double Low
                {
                    double get() { return shared->object->low; }
                    void set(double value) { shared->object->low = value; }
                }

                property double Medium
                {
                    double get() { return shared->object->medium; }
                    void set(double value) { shared->object->medium = value; }
                }

                property double High
                {
                    double get() { return shared->object->high; }
                    void set(double value) { shared->object->high = value; }
                }

                property double FirstOrderIndex
                {
                    double get() { return shared->object->firstOrderIndex; }
                    void set(double value) { shared->object->firstOrderIndex = value; }
                }

                property double TotalIndex
                {
                    double get() { return shared->object->totalIndex; }
                    void set(double value) { shared->object->totalIndex = value; }
                }
            };
        }
    }
}


