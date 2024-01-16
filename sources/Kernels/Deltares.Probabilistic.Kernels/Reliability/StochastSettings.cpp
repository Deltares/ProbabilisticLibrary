#include "StochastSettings.h"

namespace Deltares
{
	namespace Reliability
	{
		bool StochastSettings::isMinMaxDefault()
		{
			return this->MinValue <= -StandardNormal::UMax && this->MaxValue >= StandardNormal::UMax;
		}
	}
}

