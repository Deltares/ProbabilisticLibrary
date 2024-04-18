#pragma once
#include "Distribution.h"
#include "../DistributionType.h"

namespace Deltares
{
	namespace Statistics
	{
		class DistributionLibrary
		{
		public:
			static std::shared_ptr<Statistics::Distribution> getDistribution(DistributionType distributionType, bool truncated, bool inverted);
		};
	}
}

