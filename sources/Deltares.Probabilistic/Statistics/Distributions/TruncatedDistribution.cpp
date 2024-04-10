#include "TruncatedDistribution.h"

#include <cmath>

#include "DistributionFitter.h"
#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"

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
				double x = this->innerDistribution->getXFromU(stochast, getUntruncatedU(u, stochast));

				if (!std::isinf(stochast->Maximum) && !std::isnan(stochast->Maximum) && x > stochast->Maximum)
				{
					x = stochast->Maximum;
				}
				else if (!std::isinf(stochast->Minimum) && !std::isnan(stochast->Minimum) && x < stochast->Minimum)
				{
					x = stochast->Minimum;
				}

				return x;
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
			if (x < stochast->Minimum)
			{
				return 0;
			}
			else if (x > stochast->Maximum)
			{
				return 1;
			}

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

			std::vector<DistributionPropertyType> properties = getParameters();
			std::shared_ptr<DistributionFitter> distributionFitter = std::make_shared<DistributionFitter>();

			if (areEqual(properties, {DistributionPropertyType::Location, DistributionPropertyType::Scale}))
			{
				std::vector<double> minValues = { stochast->Location - 3 * stochast->Scale, 0.1 * stochast->Scale };
				std::vector<double> maxValues = { stochast->Location + 3 * stochast->Scale, 1.9 * stochast->Scale };
				std::vector<double> initValues = { stochast->Location, stochast->Scale };

				std::vector<double> parameters = distributionFitter->fitByLogLikelihood(values, this, stochast, minValues, maxValues, initValues, properties);

				stochast->Location = parameters[0];
				stochast->Scale = std::max(0.0, parameters[1]);
			}
			else if (areEqual(properties, {DistributionPropertyType::Shift, DistributionPropertyType::Scale}))
			{
				std::vector<double> minValues = { stochast->Shift - 3 * stochast->Scale, 0.1 * stochast->Scale };
				std::vector<double> maxValues = { stochast->Shift + 3 * stochast->Scale, 1.9 * stochast->Scale };
				std::vector<double> initValues = { stochast->Shift, stochast->Scale };

				std::vector<double> parameters = distributionFitter->fitByLogLikelihood(values, this, stochast, minValues, maxValues, initValues, properties);

				stochast->Shift = parameters[0];
				stochast->Scale = std::max(0.0, parameters[1]);
			}
			else if (areEqual(properties, {DistributionPropertyType::Shape, DistributionPropertyType::Scale}))
			{
				std::vector<double> minValues = { 0.1 * stochast->Shape, 0.1 * stochast->Scale };
				std::vector<double> maxValues = { 1.9 * stochast->Shape, 1.9 * stochast->Scale };
				std::vector<double> initValues = { stochast->Shift, stochast->Scale };

				std::vector<double> parameters = distributionFitter->fitByLogLikelihood(values, this, stochast, minValues, maxValues, initValues, properties);

				stochast->Shape = parameters[0];
				stochast->Scale = std::max(0.0, parameters[1]);
			}
		}

		bool TruncatedDistribution::areEqual(std::vector<DistributionPropertyType> array1, std::vector<DistributionPropertyType> array2)
		{
			if (array1.size() != array2.size())
			{
				return false;
			}

			for (int i = 0; i < array1.size(); i++)
			{
				if (array1[i] != array2[i])
				{
					return false;
				}
			}

			return true;
		}


		std::vector<DistributionPropertyType> TruncatedDistribution::getParameters()
		{
			return innerDistribution->getParameters();
		}

		double TruncatedDistribution::getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x)
		{
			Truncated truncated = getTruncatedValue(stochast);

			if (truncated.Factor == 0 || std::isinf(truncated.Factor) || std::isnan(truncated.Factor))
			{
				return nan("");
			}
			else
			{
				return log(truncated.Factor) + this->innerDistribution->getLogLikelihood(stochast, x);
			}
		}

		std::vector<double> TruncatedDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
		{
			std::vector<double> specialPoints;

			const bool hasMinimum = !std::isnan(stochast->Minimum) && !std::isinf(stochast->Minimum);
			const bool hasMaximum = !std::isnan(stochast->Maximum) && !std::isinf(stochast->Maximum);

			if (hasMinimum)
			{
				specialPoints.push_back(stochast->Minimum + Numeric::NumericSupport::getFraction(stochast->Minimum, -0.1));
				specialPoints.push_back(stochast->Minimum + Numeric::NumericSupport::getFraction(stochast->Minimum, -0.000001));
				specialPoints.push_back(stochast->Minimum);
			}

			std::vector<double> innerPoints = this->innerDistribution->getSpecialPoints(stochast);

			for (double x : innerPoints) 
			{
				if ((!hasMinimum || x > stochast->Minimum) && (!hasMaximum || x < stochast->Maximum)) 
				{
					specialPoints.push_back(x);
				}
			}

			if (hasMaximum)
			{
				specialPoints.push_back(stochast->Maximum);
				specialPoints.push_back(stochast->Maximum + Numeric::NumericSupport::getFraction(stochast->Maximum, 0.1));
			}

			return specialPoints;
		}
	}
}
