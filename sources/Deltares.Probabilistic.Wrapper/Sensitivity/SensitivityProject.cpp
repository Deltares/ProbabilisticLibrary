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


