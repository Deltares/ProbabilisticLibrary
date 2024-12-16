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

#include "../../Deltares.Probabilistic/Statistics/DiscreteValue.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public ref class DiscreteValue
            {
            private:
                SharedPointerProvider<Statistics::DiscreteValue>* shared = nullptr;

            public:
                DiscreteValue()
                {
                    shared = new SharedPointerProvider(new Statistics::DiscreteValue());
                }

                DiscreteValue(std::shared_ptr<Statistics::DiscreteValue> discreteValue)
                {
                    shared = new SharedPointerProvider(discreteValue);
                }

                DiscreteValue(double x, double amount)
                {
                    shared = new SharedPointerProvider(new Statistics::DiscreteValue());
                    shared->object->X = x;
                    shared->object->Amount = amount;
                }

                ~DiscreteValue() { this->!DiscreteValue(); }
                !DiscreteValue() { delete shared; }

                property double X
                {
                    double get() { return shared->object->X; }
                    void set(double value)
                    {
                        shared->object->X = value;
                        shared->object->setDirty();
                    }
                }

                property double Amount
                {
                    double get() { return shared->object->Amount; }
                    void set(double value)
                    {
                        shared->object->Amount = value;
                        shared->object->setDirty();
                    }
                }

                property double NormalizedAmount
                {
                    double get() { return shared->object->NormalizedAmount; }
                }

                property double CumulativeNormalizedAmount
                {
                    double get() { return shared->object->CumulativeNormalizedAmount; }
                }

                std::shared_ptr<Statistics::DiscreteValue> GetValue()
                {
                    return shared->object;
                }
            };
        }
    }
}




