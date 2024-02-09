#include "UniformDistribution.h"
#include "../StandardNormal.h"
#include "../StochastProperties.h"

#include <cmath>

namespace Deltares
{
	namespace Statistics
	{
		void UniformDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
		{
			double diff = sqrt(3) * deviation;
			stochast->Minimum = mean - diff;
			stochast->Maximum = mean + diff;
		}

		void UniformDistribution::initialize(StochastProperties* stochast, std::vector<double> values)
		{
			stochast->Minimum = values[0];
			stochast->Maximum = values[1];
		}

		bool UniformDistribution::isVarying(StochastProperties* stochast)
		{
			return stochast->Maximum > stochast->Minimum;
		}

		double UniformDistribution::getMean(StochastProperties* stochast)
		{
			return (stochast->Minimum + stochast->Maximum) / 2;
		}

		double UniformDistribution::getDeviation(StochastProperties* stochast)
		{
			double diff = stochast->Maximum - stochast->Minimum;
			return sqrt(diff * diff / 12);
		}

		double UniformDistribution::getXFromU(StochastProperties* stochast, double u)
		{
			double p = StandardNormal::getPFromU(u);

			return stochast->Minimum + p * (stochast->Maximum - stochast->Minimum);
		}

		double UniformDistribution::getUFromX(StochastProperties* stochast, double x)
		{
			if (x <= stochast->Minimum)
			{
				return -StandardNormal::UMax;
			}
			else if (x >= stochast->Maximum)
			{
				return StandardNormal::UMax;
			}
			else
			{
				double cdf = 1 - (stochast->Maximum - x) / (stochast->Maximum - stochast->Minimum);
				return StandardNormal::getUFromP(cdf);
			}
		}

		double UniformDistribution::getPDF(StochastProperties* stochast, double x)
		{
			if (stochast->Minimum == stochast->Maximum)
			{
				return x == stochast->Minimum ? 1 : 0;
			}
			else if (x >= stochast->Minimum && x <= stochast->Maximum)
			{
				return 1 / (stochast->Maximum - stochast->Minimum);
			}
			else
			{
				return 0;
			}
		}

		double UniformDistribution::getCDF(StochastProperties* stochast, double x)
		{
			if (x <= stochast->Minimum)
			{
				return 0;
			}
			else if (x >= stochast->Maximum)
			{
				return 1;
			}
			else
			{
				return 1 - ((stochast->Maximum - x) / (stochast->Maximum - stochast->Minimum));
			}
		}

		void UniformDistribution::setXAtU(StochastProperties* stochast, double x, double u, ConstantParameterType constantType)
		{
			if (constantType == Deviation) 
			{
				double p = StandardNormal::getPFromU(u);

				if (stochast->Minimum == stochast->Maximum)
				{
					stochast->Minimum = x;
					stochast->Maximum = x;
				}
				else
				{
					double currentValue = this->getXFromU(stochast, u);
					double diff = x - currentValue;

					stochast->Minimum += diff;
					stochast->Maximum += diff;
				}
			}
			else
			{
				return setXAtUByIteration(stochast, x, u, constantType);
			}
		}

		void UniformDistribution::fit(StochastProperties* stochast, std::vector<double>& values)
		{
			double min = *std::min_element(values.begin(), values.end());
			double max = *std::max_element(values.begin(), values.end());

			double diff = max - min;
			double add = diff / values.size();

			stochast->Minimum = min - add;
			stochast->Maximum = max + add;
		}

	}
}





