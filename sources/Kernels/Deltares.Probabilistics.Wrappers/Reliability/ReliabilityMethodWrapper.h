#pragma once
#include "DesignPointWrapper.h"
#include "../Model/ModelRunnerWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/ReliabilityMethod.h"
#include "../Model/MessageWrapper.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
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

