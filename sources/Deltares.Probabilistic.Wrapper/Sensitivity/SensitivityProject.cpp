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
#include "SensitivityProject.h"
#include "../Model/ModelRunner.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            delegate void ManagedSampleDelegate(std::shared_ptr<Models::ModelSample> sample);
            typedef void(__stdcall* ZDelegate) (std::shared_ptr<Models::ModelSample>);

            bool SensitivityProject::IsValid()
            {
                shared->object->stochasts.clear();

                for (size_t i = 0; i < this->Stochasts->Count; i++)
                {
                    shared->object->stochasts.push_back(this->Stochasts[i]->GetStochast());
                }

                shared->object->correlationMatrix = this->CorrelationMatrix->GetCorrelationMatrix();
                shared->object->sensitivityMethod = this->SensitivityMethod->GetNativeSensitivityMethod();

                return shared->object->isValid();
            }

            Stochast^ SensitivityProject::GetStochast()
            {
                shared->object->stochasts.clear();

                for (size_t i = 0; i < this->Stochasts->Count; i++)
                {
                    shared->object->stochasts.push_back(this->Stochasts[i]->GetStochast());
                }

                shared->object->correlationMatrix = this->CorrelationMatrix->GetCorrelationMatrix();
                shared->object->sensitivityMethod = this->SensitivityMethod->GetNativeSensitivityMethod();
                shared->object->runSettings = this->Settings->GetSettings();
                shared->object->progressIndicator = this->ProgressIndicator != nullptr ? this->ProgressIndicator->GetProgressIndicator() : nullptr;

                ZLambda zLambda = getZLambda();

                shared->object->model = std::make_shared<ZModel>(zLambda);

                const std::shared_ptr<Statistics::Stochast> stochast = shared->object->getStochast();

                this->ReleaseHandles();

                this->Stochast = gcnew Statistics::Wrappers::Stochast(stochast);

                return this->Stochast;
            }

            ZLambda SensitivityProject::getZLambda()
            {
                ManagedSampleDelegate^ fp = gcnew ManagedSampleDelegate(this, &SensitivityProject::invokeSample);
                System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
                handles->Add(handle);

                System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
                ZLambda functionPointer = static_cast<ZDelegate>(callbackPtr.ToPointer());

                return functionPointer;
            }

            void SensitivityProject::invokeSample(std::shared_ptr<Models::ModelSample> sample)
            {
                ModelSample^ sampleWrapper = gcnew ModelSample(sample);
                this->ZFunction->Invoke(sampleWrapper);
            }
        }
    }
}


