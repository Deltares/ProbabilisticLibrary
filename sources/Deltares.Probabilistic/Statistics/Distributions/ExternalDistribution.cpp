#include "ExternalDistribution.h"

namespace Deltares
{
	namespace Statistics
	{
		double ExternalDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
		{
			if (uxFunction != nullptr)
			{
				return uxFunction(u);
			}
			else
			{
				return nan("");
			}
		}
	}
}




