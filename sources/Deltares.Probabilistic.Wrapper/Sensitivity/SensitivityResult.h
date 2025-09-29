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
#include "../Statistics/Stochast.h"
#include "../Utils/SharedPointerProvider.h"
#include "SensitivityValue.h"
#include <map>

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class SensitivityResult
            {
            private:
                SharedPointerProvider<Sensitivity::SensitivityResult>* shared = nullptr;
                System::Collections::Generic::List<Wrappers::SensitivityValue^>^ values = gcnew System::Collections::Generic::List<SensitivityValue^>();

            public:
                SensitivityResult()
                {
                    shared = new SharedPointerProvider(new Deltares::Sensitivity::SensitivityResult());
                }

                SensitivityResult(std::shared_ptr<Deltares::Sensitivity::SensitivityResult> sensitivityResult, System::Collections::Generic::IList<Statistics::Wrappers::Stochast^>^ stochasts)
                {
                    shared = new SharedPointerProvider(sensitivityResult);

                    std::map<std::shared_ptr<Statistics::Stochast>, int> stochastMapping;
                    for (int i = 0; i < stochasts->Count; i++)
                    {
                        stochastMapping[stochasts[i]->GetStochast()] = i;
                    }

                    for (int i = 0; i < sensitivityResult->values.size(); i++)
                    {
                        Wrappers::SensitivityValue^ sensitivityValue = gcnew Wrappers::SensitivityValue(sensitivityResult->values[i]);
                        sensitivityValue->Parameter = stochasts[stochastMapping[sensitivityResult->values[i]->stochast]];
                        this->Values->Add(sensitivityValue);
                    }
                }

                ~SensitivityResult() { this->!SensitivityResult(); }
                !SensitivityResult() { delete shared; }

                property System::Collections::Generic::List<SensitivityValue^>^ Values
                {
                    System::Collections::Generic::List<SensitivityValue^>^ get() { return values; }
                }



                //property int Iterations
                //{
                //    bool get() { return shared->object->Iterations; }
                //    void set(bool value) { shared->object->Iterations = value; }
                //}
            };
        }
    }
}


