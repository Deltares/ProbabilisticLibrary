#include "DeterministicDistribution.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"

namespace Deltares
{
	namespace Statistics
	{
		bool DeterministicDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
		{
			return false;
		}

		void DeterministicDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
		{
			stochast->Location = mean;
			stochast->Scale = deviation;
		}

		void DeterministicDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
		{
			stochast->Location = values[0];
		}

		double DeterministicDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Location;
		}

		double DeterministicDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
		{
			return 0;
		}

		double DeterministicDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
		{
			return stochast->Location;
		}

		double DeterministicDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			return x == stochast->Location ? 1 : 0;
		}

		double DeterministicDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			if (x < stochast->Location)
			{
				return 0;
			}
			else if (x > stochast->Location)
			{
				return 1;
			}
			else
			{
				return 0.5;
			}
		}

		void DeterministicDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
		{
			stochast->Location = x;
		}

		void DeterministicDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
		{
			stochast->Location = Numeric::NumericSupport::getMean(values);
		}
	}
}



