#pragma once
#include "../Statistics/Stochast.h"
#include "../Model/ModelRunner.h"
#include "../../Deltares.Probabilistic/Sensitivity/SensitivityMethod.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            public ref class SensitivityMethod
            {
            public:
                SensitivityMethod() {  }

                virtual std::shared_ptr<Sensitivity::SensitivityMethod> GetNativeSensitivityMethod()
                {
                    return nullptr;
                };

                Statistics::Wrappers::Stochast^ GetStochast(Models::Wrappers::ModelRunner^ modelRunner);

                virtual System::Object^ GetSettings() { return nullptr; }
                virtual bool IsValid() { return false; }
                virtual void Stop()    { }
                virtual bool IsStopped() { return false; }
            };
        }
    }
}

