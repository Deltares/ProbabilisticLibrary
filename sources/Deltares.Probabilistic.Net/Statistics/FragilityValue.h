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

#include "../../Deltares.Probabilistic/Statistics/FragilityValue.h"
#include "../Model/BaseStochastPoint.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            public ref class FragilityValue : System::IComparable<FragilityValue^>
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Statistics::FragilityValue>* shared = nullptr;
                Models::Wrappers::BaseStochastPoint^ designPoint = nullptr;
            public:
                FragilityValue()
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(new Statistics::FragilityValue());
                }

                FragilityValue(std::shared_ptr<Statistics::FragilityValue> fragilityValue)
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(fragilityValue);
                }

                ~FragilityValue() { this->!FragilityValue(); }
                !FragilityValue() { delete shared; }

                property double X
                {
                    double get() { return shared->object->X; }
                    void set(double value)
                    {
                        shared->object->X = value;
                        shared->object->setDirty();
                    }
                }

                property double Reliability
                {
                    double get() { return shared->object->Reliability; }
                    void set(double value)
                    {
                        shared->object->Reliability = value;
                        shared->object->setDirty();
                    }
                }

                property Models::Wrappers::BaseStochastPoint^ DesignPoint
                {
                    Models::Wrappers::BaseStochastPoint^ get() { return this->designPoint; }
                    void set(Models::Wrappers::BaseStochastPoint^ value)
                    {
                        this->designPoint = value;
                        shared->object->designPoint = value != nullptr ?  value->getDesignPoint() : nullptr;
                    }
                }

                virtual int CompareTo(FragilityValue^ other)
                {
                    return this->X.CompareTo(other->X);
                }

                std::shared_ptr<Statistics::FragilityValue> GetValue()
                {
                    if (this->designPoint != nullptr)
                    {
                        shared->object->designPoint = this->designPoint->getDesignPoint();
                    }

                    return shared->object;
                }
            };
        }
    }
}
