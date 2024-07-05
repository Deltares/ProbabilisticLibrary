#pragma once
#include "ImportanceSamplingSettings.h"
#include "AdaptiveImportanceSamplingSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/AdaptiveImportanceSampling.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class AdaptiveImportanceSampling : public ReliabilityMethod
            {
            private:
                SharedPointerProvider<Reliability::AdaptiveImportanceSampling>* shared = new SharedPointerProvider(new Reliability::AdaptiveImportanceSampling());
            public:
                AdaptiveImportanceSampling() { }
                ~AdaptiveImportanceSampling() { this->!AdaptiveImportanceSampling(); }
                !AdaptiveImportanceSampling() { delete shared; }

                AdaptiveImportanceSamplingSettings^ Settings = gcnew AdaptiveImportanceSamplingSettings();

                System::Object^ GetSettings() override { return Settings; }

                bool IsValid() override { return Settings->IsValid(); }

                std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
                {
                    shared->object->Settings = Settings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}



