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
#include "ReliabilityProject.h"
#include "../Model/ModelRunner.h"
#include "../Model/ModelSample.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            delegate void ManagedSampleDelegate(std::shared_ptr<Models::ModelSample> sample);
            typedef void(__stdcall* ZDelegate) (std::shared_ptr<Models::ModelSample>);

            bool ReliabilityProject::IsValid()
            {
                shared->object->stochasts.clear();

                for (size_t i = 0; i < this->Stochasts->Count; i++)
                {
                    shared->object->stochasts.push_back(this->Stochasts[i]->GetStochast());
                }

                shared->object->correlationMatrix = this->CorrelationMatrix->GetCorrelationMatrix();
                shared->object->reliabilityMethod = this->ReliabilityMethod->GetReliabilityMethod();

                return shared->object->isValid();
            }

            Reliability::Wrappers::DesignPoint^ ReliabilityProject::GetDesignPoint()
            {
                shared->object->stochasts.clear();

                for (size_t i = 0; i < this->Stochasts->Count; i++)
                {
                    shared->object->stochasts.push_back(this->Stochasts[i]->GetStochast());
                }

                shared->object->correlationMatrix = this->CorrelationMatrix->GetCorrelationMatrix();
                shared->object->reliabilityMethod = this->ReliabilityMethod->GetReliabilityMethod();
                shared->object->runSettings = this->Settings->GetSettings();
                shared->object->progressIndicator = this->ProgressIndicator != nullptr ? this->ProgressIndicator->GetProgressIndicator() : nullptr;

                ZLambda zLambda = getZLambda();

                shared->object->model = std::make_shared<ZModel>(zLambda);

                const std::shared_ptr<Reliability::DesignPoint> designPoint = shared->object->getDesignPoint();

                this->ReleaseHandles();

                this->DesignPoint = gcnew Reliability::Wrappers::DesignPoint(designPoint, this->Stochasts);

                this->DesignPoint->AssignTags(this->TagRepository);

                return this->DesignPoint;
            }

            ZLambda ReliabilityProject::getZLambda()
            {
                ManagedSampleDelegate^ fp = gcnew ManagedSampleDelegate(this, &ReliabilityProject::invokeSample);
                System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
                handles->Add(handle);

                System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
                ZLambda functionPointer = static_cast<ZDelegate>(callbackPtr.ToPointer());

                return functionPointer;
            }

            void ReliabilityProject::invokeSample(std::shared_ptr<Models::ModelSample> sample)
            {
                Wrappers::ModelSample^ sampleWrapper = gcnew Wrappers::ModelSample(sample);
                this->ZFunction->Invoke(sampleWrapper);
            }
        }
    }
}


