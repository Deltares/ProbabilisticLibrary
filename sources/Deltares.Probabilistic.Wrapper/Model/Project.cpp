#include "Project.h"
#include "ModelRunner.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            delegate void ManagedSampleDelegate(std::shared_ptr<Models::ModelSample> sample);
            typedef void(__stdcall* ZDelegate) (std::shared_ptr<Models::ModelSample>);

            bool Project::IsValid()
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

            Reliability::Wrappers::DesignPoint^ Project::GetDesignPoint()
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

                this->DesignPoint = gcnew Reliability::Wrappers::DesignPoint(designPoint, this->Stochasts);

                return this->DesignPoint;
            }

            ZLambda Project::getZLambda()
            {
                ManagedSampleDelegate^ fp = gcnew ManagedSampleDelegate(this, &Project::invokeSample);
                System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
                handles->Add(handle);

                System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
                ZLambda functionPointer = static_cast<ZDelegate>(callbackPtr.ToPointer());

                return functionPointer;
            }

            void Project::invokeSample(std::shared_ptr<Models::ModelSample> sample)
            {
                ModelSample^ sampleWrapper = gcnew ModelSample(sample);
                this->ZFunction->Invoke(sampleWrapper);
            }
        }
    }
}


