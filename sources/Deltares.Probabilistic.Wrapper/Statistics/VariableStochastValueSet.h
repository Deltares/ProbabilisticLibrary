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
            using namespace Deltares::Utils::Wrappers;

            public ref class VariableStochastValueSet
            {
            private:
                SharedPointerProvider<Statistics::VariableStochastValuesSet>* shared = new SharedPointerProvider(new Statistics::VariableStochastValuesSet());

                CallBackList<VariableStochastValue^>^ stochastValues = gcnew CallBackList<VariableStochastValue^>();

                void SynchronizeStochastValues(ListOperationType listOperationType, VariableStochastValue^ histogramValue);
            public:
                VariableStochastValueSet()
                {
                    stochastValues->SetCallBack(gcnew ListCallBack<VariableStochastValue^>(this, &VariableStochastValueSet::SynchronizeStochastValues));
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








