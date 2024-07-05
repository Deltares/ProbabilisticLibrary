#pragma once

#include "DirectionalSamplingSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/DirectionalSampling.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class DirectionalSampling : public ReliabilityMethod
            {
            private:
                SharedPointerProvider<Reliability::DirectionalSampling>* shared = new SharedPointerProvider(new Reliability::DirectionalSampling());
            public:
                DirectionalSampling() {}
                ~DirectionalSampling() { this->!DirectionalSampling(); }
                !DirectionalSampling() { delete shared; }

                std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
                {
                    shared->object->Settings = Settings->GetSettings();
                    return shared->object;
                }

                DirectionalSamplingSettings^ Settings = gcnew DirectionalSamplingSettings();

                bool IsValid() override { return Settings->IsValid(); }
            };
        }
    }
}





