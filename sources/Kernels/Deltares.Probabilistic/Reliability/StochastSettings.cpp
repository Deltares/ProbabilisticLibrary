#include "StochastSettings.h"

namespace Deltares
{
	namespace Reliability
	{
		bool StochastSettings::isMinMaxDefault()
		{
			return this->MinValue <= -Statistics::StandardNormal::UMax && this->MaxValue >= Statistics::StandardNormal::UMax;
		}

		void StochastSettings::initializeForRun()
		{
			this->XMinValue = Statistics::StandardNormal::getPFromU(this->MinValue);
			this->XMaxValue = Statistics::StandardNormal::getPFromU(this->MaxValue);
		}

		void StochastSettings::setStochast(std::shared_ptr<Statistics::Stochast> stochast)
		{
			this->stochast = stochast;
		}

		double StochastSettings::getRepresentativeU(double u)
		{
			return this->stochast->getRepresentativeU(u);
		}
	}
}

