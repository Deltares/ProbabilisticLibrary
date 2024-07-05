#include "ReliabilityMethod.h"

#include "DesignPoint.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            DesignPoint^ ReliabilityMethod::GetDesignPoint(Wrappers::ModelRunner^ modelRunner)
            {
                const std::shared_ptr<Reliability::ReliabilityMethod> reliabilityMethod = this->GetReliabilityMethod();

                const std::shared_ptr<Models::ModelRunner> nativeModelRunner = modelRunner->GetModelRunner();

                nativeModelRunner->initializeForRun();

                const std::shared_ptr<Reliability::DesignPoint> nativeDesignPoint = reliabilityMethod->getDesignPoint(nativeModelRunner);

                Wrappers::DesignPoint^ designPoint = gcnew Wrappers::DesignPoint(nativeDesignPoint, modelRunner->Stochasts);

                return designPoint;
            };
        }
    }
}


