#include "DeterministicDistribution.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"

namespace Deltares
{
	namespace Statistics
	{
		bool DeterministicDistribution::isVarying(StochastProperties* stochast)
		{
			return false;
		}

		void DeterministicDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
		{
			stochast->Location = mean;
		}

		void DeterministicDistribution::initialize(StochastProperties* stochast, double* values)
		{
			stochast->Location = values[0];
		}

		double DeterministicDistribution::getMean(StochastProperties* stochast)
		{
			return stochast->Location;
		}

		double DeterministicDistribution::getDeviation(StochastProperties* stochast)
		{
			return 0;
		}

		double DeterministicDistribution::getXFromU(StochastProperties* stochast, double u)
		{
			return stochast->Location;
		}

		double DeterministicDistribution::getPDF(StochastProperties* stochast, double x)
		{
			return x == stochast->Location ? 1 : 0;
		}

		double DeterministicDistribution::getCDF(StochastProperties* stochast, double x)
		{
			return x >= stochast->Location ? 1 : 0;
		}

		void DeterministicDistribution::setXAtU(StochastProperties* stochast, double x, double u)
		{
			stochast->Location = x;
		}

		void DeterministicDistribution::fit(StochastProperties* stochast, std::vector<double> values)
		{
			stochast->Location = Numeric::NumericSupport::getMean(values);
		}
	}
}



