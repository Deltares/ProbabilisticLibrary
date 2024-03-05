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

				const std::shared_ptr<Models::ModelRunner> modelRunnerNative = modelRunner->GetModelRunner();

				modelRunnerNative->initializeForRun();

				const std::shared_ptr<Reliability::DesignPoint> designPoint = reliabilityMethod->getDesignPoint(modelRunnerNative);

				Wrappers::DesignPoint^ designPointWrapper = gcnew Wrappers::DesignPoint(designPoint, modelRunner->Stochasts);

				NativeSupport::releaseManagedObjects();

				return designPointWrapper;
			};
		}
	}
}


