#include "StochastSettings.h"

namespace Deltares
{
	namespace Reliability
	{
		bool StochastSettings::isMinMaxDefault()
		{
			return this->MinValue <= -StandardNormal::UMax && this->MaxValue >= StandardNormal::UMax;
		}

		void StochastSettings::initializeForRun()
		{
			this->XMinValue = StandardNormal::getPFromU(this->MinValue);
			this->XMaxValue = StandardNormal::getPFromU(this->MaxValue);
		}

		void StochastSettings::setStochast(Statistics::Stochast* stochast)
		{
			this->stochast = stochast;
		}

		double StochastSettings::getRepresentativeU(double u)
		{
			return this->stochast->getRepresentativeU(u);
		}
	}
}

