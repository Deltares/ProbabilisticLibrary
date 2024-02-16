#include "DiscreteDistribution.h"

#include <algorithm>
#include <cmath>

#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"


namespace Deltares
{
	namespace Statistics
	{
		bool DiscreteDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
		{
			return !stochast->DiscreteValues.empty();
		}

		bool DiscreteDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->DiscreteValues.size() > 1;
		}

		double DiscreteDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
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

		double DiscreteDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
		{
			const double delta = 0.0000001;

			if (stochast->DiscreteValues.size() == 0)
			{
				return nan("");
			}

			double prev = 0;

			for (size_t i = 0; i < stochast->DiscreteValues.size(); i++)
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

		double DiscreteDistribution::getRepresentativeU(std::shared_ptr<StochastProperties> stochast, double u)
		{
			double x = this->getXFromU(stochast, u);
			double uRepresentative = this->getUFromX(stochast, x);

			return uRepresentative;
		}

		void DiscreteDistribution::initializeForRun(std::shared_ptr<StochastProperties> stochast)
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

		double DiscreteDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
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

		double DiscreteDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
		{
			// not supported
			return 0;
		}

		void DiscreteDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
		{
			double currentMean = getMean(stochast);
			double diff = mean - currentMean;

			for (std::shared_ptr<DiscreteValue> val : stochast->DiscreteValues)
			{
				val->X += diff;
			}
		}

		double DiscreteDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			const double delta = 0.0000001;

			if (stochast->DiscreteValues.empty())
			{
				return nan("");
			}

			for (int i = 0; i < stochast->DiscreteValues.size(); i++)
			{
				if (Numeric::NumericSupport::areEqual(stochast->DiscreteValues[i]->X, x, delta))
				{
					return stochast->DiscreteValues[i]->NormalizedAmount;
				}
			}

			return 0;

		}

		double DiscreteDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			const double delta = 0.0000001;

			if (stochast->DiscreteValues.empty())
			{
				return nan("");
			}

			double p = 0;

			for (int i = 0; i < stochast->DiscreteValues.size(); i++)
			{
				if (Numeric::NumericSupport::areEqual(stochast->DiscreteValues[i]->X, x, delta))
				{
					double pLow = p;
					double pHigh = p + stochast->DiscreteValues[i]->NormalizedAmount;
					double pAverage = (pLow + pHigh) / 2;

					return pAverage;
				}
				else if (Numeric::NumericSupport::isLess(stochast->DiscreteValues[i]->X, x, delta))
				{
					p += stochast->DiscreteValues[i]->NormalizedAmount;
				}
			}

			return p;
		}

		void DiscreteDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
		{
			stochast->DiscreteValues.clear();

			std::sort(values.begin(), values.end());

			for (int i = 0; i < values.size(); i++)
			{
				if (stochast->DiscreteValues.empty() || values[i] != values[i - 1])
				{
					stochast->DiscreteValues.push_back(std::make_shared<DiscreteValue>(values[i], 1));
				}
				else
				{
					stochast->DiscreteValues.back()->Amount += 1;
				}
			}
		}
	}
}

