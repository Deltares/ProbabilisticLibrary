#pragma once
#include "Distribution.h"
#include "../DistributionType.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			enum class DistributionType;
		}

		class DistributionLibrary
		{
		public:
			static std::shared_ptr<Statistics::Distribution> getDistribution(DistributionType distributionType, bool truncated, bool inverted);
		};
	}
}

