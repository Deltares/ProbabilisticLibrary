#include "HistogramDistribution.h"

#include <algorithm>
#include <cmath>
#include "../StochastProperties.h"
#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"

#include <limits>

namespace Deltares
{
	namespace Statistics
	{
		void HistogramDistribution::initializeForRun(std::shared_ptr<StochastProperties> stochast)
		{
			std::sort(stochast->HistogramValues.begin(), stochast->HistogramValues.end(), [](std::shared_ptr<HistogramValue> val1, std::shared_ptr<HistogramValue> val2) {return val1->CompareTo(val2.get()); });

			double sum = 0;
			for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
			{
				sum += histogramValue->Amount;
			}

			for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
			{
				histogramValue->NormalizedAmount = histogramValue->Amount / sum;
			}

			double cumulative = 0;
			for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
			{
				cumulative += histogramValue->NormalizedAmount;
				histogramValue->CumulativeNormalizedAmount = cumulative;
			}
		}

		void HistogramDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
		{
			double currentMean = this->getMean(stochast);
			double diff = mean - currentMean;

			for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
			{
				histogramValue->LowerBound += diff;
				histogramValue->UpperBound += diff;
			}
		}

		bool HistogramDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
		{
			if (stochast->HistogramValues.size() == 0)
			{
				return false;
			}

			for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
			{
				if (!histogramValue->isValid())
				{
					return false;
				}
			}

			return true;
		}

		bool HistogramDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
		{
			if (stochast->HistogramValues.size() == 0)
			{
				return false;
			}

			return true;
		}

		double HistogramDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
		{
			double sum = 0;
			double sumWeights = 0;

			for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
			{
				sum += histogramValue->getCenter() * histogramValue->NormalizedAmount;
				sumWeights += histogramValue->NormalizedAmount;
			}

			return sum / sumWeights;
		}

		double HistogramDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
		{
			double mean = getMean(stochast);

			double sumVariances = 0;
			double sumWeights = 0;

			for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
			{
				const double diff = histogramValue->getCenter() - mean;

				sumVariances += diff * diff * histogramValue->Amount;
				sumWeights += histogramValue->Amount;
			}

			if (sumWeights == 0)
			{
				return 0;
			}
			else if (sumWeights <= 1)
			{
				return sqrt(sumVariances / sumWeights);
			}
			else
			{
				return sqrt(sumVariances / (sumWeights - 1));
			}
		}

		double HistogramDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
		{
			double pu = StandardNormal::getPFromU(u);

			if (stochast->HistogramValues.empty())
			{
				return nan("");
			}

			for (size_t i = 0; i < stochast->HistogramValues.size(); i++)
			{
				if (stochast->HistogramValues[i]->CumulativeNormalizedAmount < pu)
				{
					continue;
				}
				else
				{
					double previousCumulativeAmount = i > 0 ? stochast->HistogramValues[i - 1]->CumulativeNormalizedAmount : 0;
					double offset = (pu - previousCumulativeAmount) / (stochast->HistogramValues[i]->CumulativeNormalizedAmount - previousCumulativeAmount);

					// linear interpolation within range
					return offset * stochast->HistogramValues[i]->UpperBound + (1 - offset) * stochast->HistogramValues[i]->LowerBound;
				}
			}

			return stochast->HistogramValues[stochast->HistogramValues.size() - 1]->UpperBound;
		}

		double HistogramDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
		{
			double cdf = this->getCDF(stochast, x);

			return StandardNormal::getUFromP(cdf);
		}

		double HistogramDistribution::getSizeForEmptySizedRange(std::shared_ptr<StochastProperties> stochast)
		{
			double max = std::numeric_limits<double>::infinity();

			for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
			{
				const double size = histogramValue->getSize();
				if (size < max && size > 0)
				{
					max = size;
				}
			}

			if (!std::isinf(max))
			{
				return max;
			}
			else 
			{
				return 1;
			}
		}

		double HistogramDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			const double delta = 0.0000001;

			getSizeForEmptySizedRange(stochast);

			for (size_t i = 0; i < stochast->HistogramValues.size(); i++)
			{
				if (Numeric::NumericSupport::areEqual(stochast->HistogramValues[i]->LowerBound, stochast->HistogramValues[i]->UpperBound, delta) && 
					Numeric::NumericSupport::areEqual(stochast->HistogramValues[i]->LowerBound, x, delta))
				{
					return stochast->HistogramValues[i]->NormalizedAmount / getSizeForEmptySizedRange(stochast);
				}
				else
				{
					if (stochast->HistogramValues[i]->contains(x))
					{
						return stochast->HistogramValues[i]->NormalizedAmount / stochast->HistogramValues[i]->getSize();
					}
				}
			}

			return 0;
		}

		double HistogramDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			if (stochast->HistogramValues.empty())
			{
				return nan("");
			}

			double p = 0;

			for (size_t i = 0; i < stochast->HistogramValues.size(); i++)
			{
				if (stochast->HistogramValues[i]->LowerBound < x)
				{
					if (stochast->HistogramValues[i]->UpperBound < x)
					{
						p += stochast->HistogramValues[i]->NormalizedAmount;
					}
					else
					{
						double offset = (x - stochast->HistogramValues[i]->LowerBound) / (stochast->HistogramValues[i]->UpperBound - stochast->HistogramValues[i]->LowerBound);
						p += stochast->HistogramValues[i]->NormalizedAmount * offset;
					}
				}
			}

			return p;
		}

		void HistogramDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
		{
			// TODO
		}
	}
}




