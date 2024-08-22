#pragma once

#include "../Utils/SharedPointerProvider.h"
#include "../Statistics/Stochast.h"
#include "../Sensitivity/SensitivityMethod.h"
#include "../Reliability/DesignPoint.h"
#include "../../Deltares.Probabilistic/Sensitivity/SensitivityProject.h"
#include "../Statistics/CorrelationMatrix.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            public ref class SensitivityProject
            {
            private:
                SharedPointerProvider<Sensitivity::SensitivityProject>* shared = new Utils::Wrappers::SharedPointerProvider(new Sensitivity::SensitivityProject());
                ZLambda getZLambda();
                void invokeSample(std::shared_ptr<Models::ModelSample> sample);
                System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();
            public:
                SensitivityProject() { }
                ~SensitivityProject() { this->!SensitivityProject(); }
                !SensitivityProject()
                {
                    delete shared;
                }

                void ReleaseHandles()
                {
                    for (int i = 0; i < handles->Count; i++)
                    {
                        handles[i].Free();
                    }

                    handles->Clear();
                    shared->object->model->releaseCallBacks();
                }

                System::Collections::Generic::List<Stochast^>^ Stochasts = gcnew System::Collections::Generic::List<Statistics::Wrappers::Stochast^>();
                CorrelationMatrix^ CorrelationMatrix = gcnew Statistics::Wrappers::CorrelationMatrix();

                ZSampleDelegate^ ZFunction = nullptr;

                TagRepository^ TagRepository = nullptr;

                Sensitivity::Wrappers::SensitivityMethod^ SensitivityMethod = nullptr;

                RunSettings^ Settings = gcnew RunSettings();
                ProgressIndicator^ ProgressIndicator = nullptr;

                bool IsValid();
                Stochast^ GetStochast();

                Stochast^ Stochast = nullptr;
            };
        }
    }
}

