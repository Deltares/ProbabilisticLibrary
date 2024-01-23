#include "TruncatedDistribution.h"

#include <cmath>

#include "../StandardNormal.h"

namespace Deltares
{
	namespace Statistics
	{
		struct Truncated
		{
			double LowerProbability = 0;
			double UpperProbability = 1;
			double Factor = 1;;
		};

		Truncated TruncatedDistribution::GetTruncatedValue(StochastProperties* stochast)
		{
			// detect exceeding probability of limits if it were a normal standard distribution
			double p = GetProbability(stochast, true);
			double q = GetProbability(stochast, false);

			// the shape of the distribution must be multiplied with a factor, so that the truncated shape has a total area of 1
			double factor = 1.0 / (1 - p - q);

			Truncated truncated;
			truncated.LowerProbability = p;
			truncated.UpperProbability = q;
			truncated.Factor = factor;

			return truncated;
		}

		double TruncatedDistribution::GetProbability(StochastProperties* stochast, bool isMinimum)
		{
			double value = isMinimum ? stochast->Minimum : stochast->Maximum;

			if (isnan(value) || isinf(value))
			{
				return 0;
			}
			else
			{
				double u = this->innerDistribution->getUFromX(stochast, value);
				if (isnan(u))
				{
					return 0;
				}
				else
				{
					return isMinimum ? StandardNormal::getPFromU(u) : StandardNormal::getQFromU(u);
				}
			}
		}

		double TruncatedDistribution::GetUntruncatedU(double u, StochastProperties* stochast)
		{
			double p = StandardNormal::getPFromU(u);
			double q = StandardNormal::getQFromU(u);

			Truncated truncated = GetTruncatedValue(stochast);

			if (q < 0.5)
			{
				// zt is the u-value in the truncated distribution, which has the same exceeding probability as u in the standard normal distribution
				// the area in the truncated distribution is: factor * (qzt - qb) = qz
				double pTruncatedExceeding = q / truncated.Factor + truncated.UpperProbability;
				return StandardNormal::getUFromQ(pTruncatedExceeding);
			}
			else
			{
				// zt is the u-value in the truncated distribution, which has the same exceeding probability as u in the standard normal distribution
				// the area in the truncated distribution is: factor * (qzt - pa) = pz
				double pTruncatedNonExceeding = p / truncated.Factor + truncated.LowerProbability;
				return StandardNormal::getUFromP(pTruncatedNonExceeding);
			}
		}

		bool TruncatedDistribution::isVarying(StochastProperties* stochast)
		{
			return stochast->Minimum != stochast->Maximum && this->innerDistribution->isVarying(stochast);
		}

		double TruncatedDistribution::getMean(StochastProperties* stochast)
		{
			return this->innerDistribution->getMean(stochast);
		}

		double TruncatedDistribution::getDeviation(StochastProperties* stochast)
		{
			return this->innerDistribution->getDeviation(stochast);
		}

		void TruncatedDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
		{
			this->innerDistribution->setMeanAndDeviation(stochast, mean, deviation);
		}

		double TruncatedDistribution::getXFromU(StochastProperties* stochast, double u)
		{
			if (stochast->Minimum == stochast->Maximum)
			{
				return stochast->Minimum;
			}
			else
			{
				return this->innerDistribution->getXFromU(stochast, GetUntruncatedU(u, stochast));
			}
		}

		double TruncatedDistribution::getUFromX(StochastProperties* stochast, double x)
		{
			if (stochast->Minimum == stochast->Maximum)
			{
				return 0;
			}
			else if (x <= stochast->Minimum)
			{
				return -StandardNormal::BetaMax;
			}
			else if (x >= stochast->Maximum)
			{
				return StandardNormal::BetaMax;
			}
			else
			{
				Truncated truncated = GetTruncatedValue(stochast);

				double u = this->innerDistribution->getUFromX(stochast, x);
				double q = StandardNormal::getQFromU(u);

				double qTruncated = (q - truncated.UpperProbability) * truncated.Factor;

				return StandardNormal::getUFromQ(qTruncated);
			}

		}
	}
}
