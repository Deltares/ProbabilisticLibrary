#pragma once

#include "../Utils/SharedPointerProvider.h"
#include "../Statistics/Stochast.h"
#include "../Reliability/ReliabilityMethod.h"
#include "../Reliability/DesignPoint.h"
#include "../../Deltares.Probabilistic/Model/Project.h"
#include "../Statistics/CorrelationMatrix.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            public ref class Project
            {
            private:
                SharedPointerProvider<Models::Project>* shared = new Utils::Wrappers::SharedPointerProvider(new Models::Project());
                ZLambda getZLambda();
                void invokeSample(std::shared_ptr<Models::ModelSample> sample);
                System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();
            public:
                Project() { }
                ~Project() { this->!Project(); }
                !Project()
                {
                    for (int i = 0; i < handles->Count; i++)
                    {
                        handles[i].Free();
                    }

                    delete shared;
                }

                System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ Stochasts = gcnew System::Collections::Generic::List<Statistics::Wrappers::Stochast^>();
                Statistics::Wrappers::CorrelationMatrix^ CorrelationMatrix = gcnew Statistics::Wrappers::CorrelationMatrix();

                ZSampleDelegate^ ZFunction = nullptr;

                Reliability::Wrappers::ReliabilityMethod^ ReliabilityMethod = nullptr;

                Models::Wrappers::RunSettings^ Settings = gcnew RunSettings();
                Models::Wrappers::ProgressIndicator^ ProgressIndicator = nullptr;

                bool IsValid();
                Reliability::Wrappers::DesignPoint^ GetDesignPoint();

                Reliability::Wrappers::DesignPoint^ DesignPoint = nullptr;
            };
        }
    }
}

