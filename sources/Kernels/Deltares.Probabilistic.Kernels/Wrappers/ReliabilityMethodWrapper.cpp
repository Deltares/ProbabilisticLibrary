#include "../pch.h"
#include "ReliabilityMethodWrapper.h"

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
			};
		}
	}
}

