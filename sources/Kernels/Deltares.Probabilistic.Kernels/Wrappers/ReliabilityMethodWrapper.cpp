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
				ReliabilityMethod* reliabilityMethodNative = this->GetReliabilityMethod();
				Models::ZModelRunner* modelRunnerNative = modelRunner->GetModelRunner();

				modelRunnerNative->initializeForRun();

				DesignPoint* designPoint = reliabilityMethodNative->getDesignPoint(modelRunnerNative);

				DesignPointWrapper^ designPointWrapper = gcnew DesignPointWrapper(designPoint, modelRunner->Stochasts);

				NativeSupport::releaseManagedObjects();

				return designPointWrapper;
			};
		}
	}
}


