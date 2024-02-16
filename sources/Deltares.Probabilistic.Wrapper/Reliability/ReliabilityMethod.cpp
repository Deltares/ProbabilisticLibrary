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
				std::shared_ptr<Reliability::ReliabilityMethod> reliabilityMethodNative = this->GetReliabilityMethod();
				std::shared_ptr<Models::ModelRunner> modelRunnerNative = modelRunner->GetModelRunner();

				modelRunnerNative->initializeForRun();

				std::shared_ptr<Reliability::DesignPoint> designPoint = reliabilityMethodNative->getDesignPoint(modelRunnerNative);

				Wrappers::DesignPoint^ designPointWrapper = gcnew Wrappers::DesignPoint(designPoint, modelRunner->Stochasts);

				NativeSupport::releaseManagedObjects();

				return designPointWrapper;
			};
		}
	}
}


