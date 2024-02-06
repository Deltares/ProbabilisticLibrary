#include "ReliabilityMethodWrapper.h"

#include "DesignPointWrapper.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			DesignPointWrapper^ ReliabilityMethodWrapper::GetDesignPoint(ModelRunnerWrapper^ modelRunner)
			{
				std::shared_ptr<Reliability::ReliabilityMethod> reliabilityMethodNative = this->GetReliabilityMethod();
				std::shared_ptr<Models::ModelRunner> modelRunnerNative = modelRunner->GetModelRunner();

				modelRunnerNative->initializeForRun();

				std::shared_ptr<DesignPoint> designPoint = reliabilityMethodNative->getDesignPoint(modelRunnerNative);

				DesignPointWrapper^ designPointWrapper = gcnew DesignPointWrapper(designPoint, modelRunner->Stochasts);

				NativeSupport::releaseManagedObjects();

				return designPointWrapper;
			};
		}
	}
}


