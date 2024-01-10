#pragma once
#include "DesignPointWrapper.h"
#include "ModelRunnerWrapper.h"
#include "../Reliability/ReliabilityMethod.h"

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

				virtual ReliabilityMethod* GetReliabilityMethod()
				{
					return nullptr;
				};

				DesignPointWrapper^ GetDesignPoint(ModelRunnerWrapper^ modelRunner);
			};
		}
	}
}

