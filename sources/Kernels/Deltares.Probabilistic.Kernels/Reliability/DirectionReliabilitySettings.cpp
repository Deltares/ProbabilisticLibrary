#include "DirectionReliabilitySettings.h"

namespace Deltares
{
	namespace Reliability
	{
		bool DirectionReliabilitySettings::IsValid()
		{
			return this->Dsdu >= 0.01 && EpsilonUStepSize >= 0.00001 && EpsilonZStepSize >= 0.00001 && this->MaximumIterations >= 1 && this->MaximumLengthU >= 1;
		}
	}
}

