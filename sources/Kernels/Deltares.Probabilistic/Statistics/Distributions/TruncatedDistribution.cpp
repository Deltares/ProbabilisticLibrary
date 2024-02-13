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
			double Factor = 1;
		};

		Truncated TruncatedDistribution::getTruncatedValue(std::shared_ptr<StochastProperties> stochast)
		{
			// detect exceeding probability of limits if it were a normal standard distribution
			double p = getProbability(stochast, true);
			double q = getProbability(stochast, false);

			// the shape of the distribution must be multiplied with a factor, so that the truncated shape has a total area of 1
			double factor = 1.0 / (1 - p - q);

			Truncated truncated;
			truncated.LowerProbability = p;
			truncated.UpperProbability = q;
			truncated.Factor = factor;

			return truncated;
		}

		double TruncatedDistribution::getProbability(std::shared_ptr<StochastProperties> stochast, bool isMinimum)
		{
			double value = isMinimum ? stochast->Minimum : stochast->Maximum;

			if (std::isnan(value) || std::isinf(value))
			{
				return 0;
			}
			else
			{
				double u = this->innerDistribution->getUFromX(stochast, value);
				if (std::isnan(u))
				{
					return 0;
				}
				else
				{
					return isMinimum ? StandardNormal::getPFromU(u) : StandardNormal::getQFromU(u);
				}
			}
		}

		double TruncatedDistribution::getUntruncatedU(double u, std::shared_ptr<StochastProperties> stochast)
		{
			double p = StandardNormal::getPFromU(u);
			double q = StandardNormal::getQFromU(u);

			Truncated truncated = getTruncatedValue(stochast);

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

		bool TruncatedDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Minimum != stochast->Maximum && this->innerDistribution->isVarying(stochast);
		}

		bool TruncatedDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Minimum <= stochast->Maximum && this->innerDistribution->isValid(stochast);
		}

		double TruncatedDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
		{
			return this->innerDistribution->getMean(stochast);
		}

		double TruncatedDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
		{
			return this->innerDistribution->getDeviation(stochast);
		}

		void TruncatedDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
		{
			this->innerDistribution->setMeanAndDeviation(stochast, mean, deviation);
		}

		void TruncatedDistribution::setShift(std::shared_ptr<StochastProperties> stochast, double shift, bool inverted)
		{
			this->innerDistribution->setShift(stochast, shift, inverted);
		}

		double TruncatedDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
		{
			if (stochast->Minimum == stochast->Maximum)
			{
				return stochast->Minimum;
			}
			else
			{
				return this->innerDistribution->getXFromU(stochast, getUntruncatedU(u, stochast));
			}
		}

		double TruncatedDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
		{
			if (stochast->Minimum == stochast->Maximum)
			{
				return 0;
			}
			else if (x <= stochast->Minimum)
			{
				return -StandardNormal::UMax;
			}
			else if (x >= stochast->Maximum)
			{
				return StandardNormal::UMax;
			}
			else
			{
				Truncated truncated = getTruncatedValue(stochast);

				double u = this->innerDistribution->getUFromX(stochast, x);
				double q = StandardNormal::getQFromU(u);

				double qTruncated = (q - truncated.UpperProbability) * truncated.Factor;

				return StandardNormal::getUFromQ(qTruncated);
			}
		}

		double TruncatedDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			if (stochast->Minimum == stochast->Maximum)
			{
				return x == stochast->Minimum ? 1 : 0;
			}
			else if (x < stochast->Minimum || x > stochast->Maximum)
			{
				return 0;
			}

			Truncated truncated = getTruncatedValue(stochast);
			return truncated.Factor * this->innerDistribution->getPDF(stochast, x);
		}

		double TruncatedDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			double u = this->getUFromX(stochast, x);

			return StandardNormal::getPFromU(u);
		}

		void TruncatedDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
		{
			this->innerDistribution->setXAtU(stochast, x, getUntruncatedU(u, stochast), constantType);
		}

		void TruncatedDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
		{
			// perform the fit without truncation
			this->innerDistribution->fit(stochast, values);

			// sets minimum and maximum
			double min = *std::min_element(values.begin(), values.end());
			double max = *std::max_element(values.begin(), values.end());

			double diff = max - min;
			double add = diff / values.size();

			stochast->Minimum = min - add;
			stochast->Maximum = max + add;

			// find properties which are fitted
			//DistributionPropertyType[] properties = GetFittedProperties();

			//if (AreEqual(properties, new[] { DistributionPropertyType.Mean, DistributionPropertyType.Deviation }))
			//{
			//	double[] parameters = DistributionFitter.FitByLogLikelihood(
			//		x,
			//		this,
			//		stochast,
			//		new[] { stochast.Mean - 3 * stochast.Deviation, 0.1 * stochast.Deviation },
			//		new[] { stochast.Mean + 3 * stochast.Deviation, 1.9 * stochast.Deviation },
			//		new[] { stochast.Mean, stochast.Deviation },
			//		properties);

			//	stochast.Mean = parameters[0];
			//	stochast.Deviation = Math.Max(0, parameters[1]);
			//}
			//else if (AreEqual(properties, new[] { DistributionPropertyType.Shift, DistributionPropertyType.Scale }))
			//{
			//	double[] parameters = DistributionFitter.FitByLogLikelihood(
			//		x,
			//		this,
			//		stochast,
			//		new[] { stochast.Shift - 3 * stochast.Scale, 0.1 * stochast.Scale },
			//		new[] { stochast.Shift + 3 * stochast.Scale, 1.9 * stochast.Scale },
			//		new[] { stochast.Shift, stochast.Scale },
			//		properties);

			//	stochast.Shift = parameters[0];
			//	stochast.Scale = Math.Max(0, parameters[1]);
			//}
			//else if (AreEqual(properties, new[] { DistributionPropertyType.Shape, DistributionPropertyType.Scale }))
			//{
			//	double[] parameters = DistributionFitter.FitByLogLikelihood(
			//		x,
			//		this,
			//		stochast,
			//		new[] { 0.1 * stochast.Shape, 0.1 * stochast.Scale },
			//		new[] { 1.9 * stochast.Shape, 1.9 * stochast.Scale },
			//		new[] { stochast.Shift, stochast.Scale },
			//		properties);

			//	stochast.Shape = parameters[0];
			//	stochast.Scale = Math.Max(0, parameters[1]);
			//}
		}

	}
}
