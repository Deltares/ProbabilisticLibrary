#include "ReliabilityMethod.h"
#include "../Statistics/StandardNormal.h"
#include "../Model/DesignPoint.h"
#include "../Model/ConvergenceReport.h"

#include <cmath>

namespace Deltares
{
	namespace Reliability
	{
		int ReliabilityMethod::getZFactor(double z)
		{
			return std::isnan(z) || z > 0.0 ? 1 : -1;
		}

		bool ReliabilityMethod::isStopped()
		{
			return this->stopped;
		}

		void ReliabilityMethod::Stop()
		{
			this->stopped = true;
		}
	}
}


