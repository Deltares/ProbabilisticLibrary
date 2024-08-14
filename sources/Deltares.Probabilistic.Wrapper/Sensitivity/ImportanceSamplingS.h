#pragma once

#include "ImportanceSamplingSettingsS.h"
#include "SensitivityMethod.h"
#include "../../Deltares.Probabilistic/Sensitivity/ImportanceSamplingS.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            public ref class ImportanceSamplingS : public SensitivityMethod
            {
            private:
                SharedPointerProvider<Sensitivity::ImportanceSamplingS>* shared = new SharedPointerProvider(new Sensitivity::ImportanceSamplingS());
            public:
                ImportanceSamplingS() { }
                ~ImportanceSamplingS() { this->!ImportanceSamplingS(); }
                !ImportanceSamplingS() { delete shared; }

                ImportanceSamplingSettingsS^ Settings = gcnew ImportanceSamplingSettingsS();

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



