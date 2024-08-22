#include "SensitivityMethod.h"

#include "../Model/ModelRunner.h"
#include "../Statistics/Stochast.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            Statistics::Wrappers::Stochast^ SensitivityMethod::GetStochast(Models::Wrappers::ModelRunner^ modelRunner)
            {
                const std::shared_ptr<Sensitivity::SensitivityMethod> sensitivityMethod = this->GetNativeSensitivityMethod();

                const std::shared_ptr<Models::ModelRunner> nativeModelRunner = modelRunner->GetModelRunner();

                nativeModelRunner->initializeForRun();

                const std::shared_ptr<Statistics::Stochast> nativeStochast = sensitivityMethod->getStochast(nativeModelRunner);

                Statistics::Wrappers::Stochast^ stochast = gcnew Statistics::Wrappers::Stochast(nativeStochast);

                return stochast;
            };
        }
    }
}


