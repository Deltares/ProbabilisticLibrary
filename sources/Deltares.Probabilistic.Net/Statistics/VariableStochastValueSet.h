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
#include "DistributionType.h"
#include "VariableStochastValue.h"
#include "BaseStochast.h"

#include "../../Deltares.Probabilistic/Statistics/VariableStochastValueSet.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Utils/CallBackList.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            public ref class VariableStochastValueSet
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Statistics::VariableStochastValuesSet>* shared = new Utils::Wrappers::SharedPointerProvider(new Statistics::VariableStochastValuesSet());

                Utils::Wrappers::CallBackList<VariableStochastValue^>^ stochastValues = gcnew Utils::Wrappers::CallBackList<VariableStochastValue^>();

                void SynchronizeStochastValues(Utils::Wrappers::ListOperationType listOperationType, VariableStochastValue^ histogramValue);
            public:
                VariableStochastValueSet()
                {
                    stochastValues->SetCallBack(gcnew Utils::Wrappers::ListCallBack<VariableStochastValue^>(this, &VariableStochastValueSet::SynchronizeStochastValues));
                }
                ~VariableStochastValueSet() { this->!VariableStochastValueSet(); }
                !VariableStochastValueSet() { delete shared; }

                property System::Collections::Generic::IList<VariableStochastValue^>^ StochastValues
                {
                    System::Collections::Generic::IList<VariableStochastValue^>^ get() { return stochastValues; }
                }

                bool IsVarying(Wrappers::DistributionType distributionType)
                {
                    return shared->object->isVarying(DistributionTypeConverter::getNativeDistributionType(distributionType));
                }

                std::shared_ptr<Statistics::VariableStochastValuesSet> GetValue()
                {
                    shared->object->StochastValues.clear();

                    for (int i = 0; i < stochastValues->Count; i++)
                    {
                        shared->object->StochastValues.push_back(this->stochastValues[i]->GetValue());
                    }

                    return shared->object;
                }
            };
        }
    }
}








