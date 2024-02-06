#pragma once
#include "DesignPointWrapper.h"
#include "../Model/ModelRunnerWrapper.h"
#include "../../Deltares.Probabilistic/Reliability/ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			public ref class ReliabilityMethodWrapper
			{
			public:
				ReliabilityMethodWrapper() {  }

				virtual std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod()
				{
					return nullptr;
				};

				DesignPointWrapper^ GetDesignPoint(ModelRunnerWrapper^ modelRunner);
			};
		}
	}
}

