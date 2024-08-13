#pragma once

#include "CrudeMonteCarloSettingsS.h"
#include "SensitivityMethod.h"
#include "../../Deltares.Probabilistic/Sensitivity/CrudeMonteCarloS.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            public ref class CrudeMonteCarloS : public SensitivityMethod
            {
            private:
                SharedPointerProvider<Sensitivity::CrudeMonteCarloS>* shared = new SharedPointerProvider(new Sensitivity::CrudeMonteCarloS());
            public:
                CrudeMonteCarloS() { }
                ~CrudeMonteCarloS() { this->!CrudeMonteCarloS(); }
                !CrudeMonteCarloS() { delete shared; }

                CrudeMonteCarloSettingsS^ Settings = gcnew CrudeMonteCarloSettingsS();

                System::Object^ GetSettings() override { return Settings; }

                bool IsValid() override { return Settings->IsValid(); }

                std::shared_ptr<Sensitivity::SensitivityMethod> GetNativeSensitivityMethod() override
                {
                    shared->object->Settings = Settings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}



