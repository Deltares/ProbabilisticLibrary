#include "DiscreteDistribution.h"

#include <algorithm>
#include <cmath>

#include "../StandardNormal.h"


namespace Deltares
{
	namespace Statistics
	{
		bool DiscreteDistribution::isVarying(StochastProperties* stochast)
		{
			return stochast->DiscreteValues.size() > 1;
		}

		double DiscreteDistribution::getXFromU(StochastProperties* stochast, double u)
		{
			const double delta = 0.0000001;

			if (stochast->DiscreteValues.size() == 0)
			{
				return nan("");
			}

			const double p = StandardNormal::getPFromU(u);

			for (int i = 0; i < stochast->DiscreteValues.size(); i++)
			{
				if (stochast->DiscreteValues[i]->CumulativeNormalizedAmount >= p - delta)
				{
					return stochast->DiscreteValues[i]->X;
				}
			}

			return stochast->DiscreteValues.back()->X;
		}

		double DiscreteDistribution::getUFromX(StochastProperties* stochast, double x)
		{
			const double delta = 0.0000001;

			if (stochast->DiscreteValues.size() == 0)
			{
				return nan("");
			}

			double prev = 0;

			for (int i = 0; i < stochast->DiscreteValues.size(); i++)
			{
				double cum = stochast->DiscreteValues[i]->CumulativeNormalizedAmount;

				if (x < stochast->DiscreteValues[i]->X)
				{
					return StandardNormal::getUFromP(prev);
				}
				else if (x == stochast->DiscreteValues[i]->X)
				{
					double p = (prev + cum) / 2;
					return StandardNormal::getUFromP(p);
				}
				else
				{
					prev = cum;
				}
			}

			return StandardNormal::getUFromP(prev);
		}

		double DiscreteDistribution::getRepresentativeU(StochastProperties* stochast, double u)
		{
			double x = this->getXFromU(stochast, u);
			double uRepresentative = this->getUFromX(stochast, x);

			return uRepresentative;
		}

		void DiscreteDistribution::initializeForRun(StochastProperties* stochast)
		{
			std::sort(stochast->DiscreteValues.begin(), stochast->DiscreteValues.end(), [](std::shared_ptr<DiscreteValue> val1, std::shared_ptr<DiscreteValue> val2) {return val1->X < val2->X; });

			double sum = 0;
			for (std::shared_ptr<DiscreteValue> discreteValue : stochast->DiscreteValues)
			{
				sum += discreteValue->Amount;
			}

			for (std::shared_ptr<DiscreteValue> discreteValue : stochast->DiscreteValues)
			{
				discreteValue->NormalizedAmount = discreteValue->Amount / sum;
			}

			double cumulative = 0;
			for (std::shared_ptr<DiscreteValue> discreteValue : stochast->DiscreteValues)
			{
				cumulative += discreteValue->NormalizedAmount;
				discreteValue->CumulativeNormalizedAmount = cumulative;
			}
		}

		double DiscreteDistribution::getMean(StochastProperties* stochast)
		{
			double sumX = 0;
			double sumAmounts = 0;

			for (std::shared_ptr<DiscreteValue> val : stochast->DiscreteValues)
			{
				sumX += val->X * val->Amount;
				sumAmounts += val->Amount;
			}

			return sumX / sumAmounts;
		}

		double DiscreteDistribution::getDeviation(StochastProperties* stochast)
		{
			// not supported
			return 0;
		}

		void DiscreteDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
		{
			double currentMean = getMean(stochast);
			double diff = mean - currentMean;

			for (std::shared_ptr<DiscreteValue> val : stochast->DiscreteValues)
			{
				val->X += diff;
			}
		}
	}
}

