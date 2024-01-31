#include "ReliabilityMethodWrapper.h"

#include "DesignPointWrapper.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			DesignPointWrapper^ ReliabilityMethodWrapper::GetDesignPoint(ModelRunnerWrapper^ modelRunner)
			{
				std::shared_ptr<Reliability::ReliabilityMethod> reliabilityMethodNative = this->GetReliabilityMethod();
				std::shared_ptr<Models::ZModelRunner> modelRunnerNative = modelRunner->GetModelRunner();

				modelRunnerNative->initializeForRun();

				std::shared_ptr<DesignPoint> designPoint = reliabilityMethodNative->getDesignPoint(modelRunnerNative);

				DesignPointWrapper^ designPointWrapper = gcnew DesignPointWrapper(designPoint, modelRunner->Stochasts);

				NativeSupport::releaseManagedObjects();

				return designPointWrapper;
			};
		}
	}
}


