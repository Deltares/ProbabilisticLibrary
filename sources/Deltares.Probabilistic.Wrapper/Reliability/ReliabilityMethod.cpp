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
				shared = new SharedPointerProvider(this->GetReliabilityMethod());

				const std::shared_ptr<Models::ModelRunner> modelRunnerNative = modelRunner->GetModelRunner();

				modelRunnerNative->initializeForRun();

				const std::shared_ptr<Reliability::DesignPoint> designPoint = shared->object->getDesignPoint(modelRunnerNative);

				Wrappers::DesignPoint^ designPointWrapper = gcnew Wrappers::DesignPoint(designPoint, modelRunner->Stochasts);

				NativeSupport::releaseManagedObjects();

				delete shared;
				shared = nullptr;

				return designPointWrapper;
			};
		}
	}
}


