#include "LogNormalDistribution.h"
#include "../StochastProperties.h"
#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include <cmath>
#include <numbers>

namespace Deltares
{
	namespace Statistics
	{
		void LogNormalDistribution::initialize(StochastProperties* stochast, std::vector<double> values)
		{
			stochast->Shift = values[2];
			setMeanAndDeviation(stochast, values[0], values[1]);
		}

		bool LogNormalDistribution::isVarying(StochastProperties* stochast)
		{
			return stochast->Scale > 0;
		}

		double LogNormalDistribution::getMean(StochastProperties* stochast)
		{
			if (isinf(stochast->Location) && !isnan(this->requestedMean))
			{
				return this->requestedMean;
			}
			else if (isinf(stochast->Location))
			{
				return stochast->Shift;
			}
			else
			{
				return exp(stochast->Location + 0.5 * stochast->Scale * stochast->Scale) + stochast->Shift;
			}
		}

		double LogNormalDistribution::getDeviation(StochastProperties* stochast)
		{
			if (isinf(stochast->Location) && !isnan(this->requestedDeviation))
			{
				return this->requestedDeviation;
			}
			else 
			{
				double p = sqrt(exp(stochast->Scale * stochast->Scale) - 1);

				double mean = getMean(stochast);

				return p * (mean - stochast->Shift);
			}
		}

		void LogNormalDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
		{
			if (mean <= stochast->Shift)
			{
				stochast->Scale = 0;
				stochast->Location = -std::numeric_limits<float>::infinity();

				this->requestedMean = mean;
				this->requestedDeviation = deviation;
			}
			else
			{
				double p = deviation / (mean - stochast->Shift);
				stochast->Scale = sqrt(log(1 + p * p));
				stochast->Location = log(mean - stochast->Shift) - 0.5 * stochast->Scale * stochast->Scale;

				this->requestedMean = nan("");
				this->requestedDeviation = nan("");
			}
		}

		void LogNormalDistribution::setShift(StochastProperties* stochast, double shift, bool inverted)
		{
			bool useRequestedValues = isinf(stochast->Location) && !isnan(this->requestedMean);

			double oldMean = useRequestedValues ? this->requestedMean : this->getMean(stochast);
			double oldDeviation = useRequestedValues ? this->requestedDeviation : this->getDeviation(stochast);

			if (inverted)
			{
				oldMean = 2 * stochast->Shift - oldMean;
			}

			stochast->Shift = shift;

			if (inverted)
			{
				oldMean = 2 * stochast->Shift - oldMean;
			}

			this->setMeanAndDeviation(stochast, oldMean, oldDeviation);
		}

		double LogNormalDistribution::getXFromU(StochastProperties* stochast, double u)
		{
			return exp(stochast->Location + u * stochast->Scale) + stochast->Shift;
		}

		double LogNormalDistribution::getUFromX(StochastProperties* stochast, double x)
		{
			if (!this->isVarying(stochast))
			{
				return 0;
			}
			else
			{
				return (log(x - stochast->Shift) - stochast->Location) / stochast->Scale;
			}
		}

		double LogNormalDistribution::getPDF(StochastProperties* stochast, double x)
		{
			if (x <= stochast->Shift)
			{
				return 0;
			}
			else
			{
				double sigma = stochast->Scale;
				double mu = stochast->Location;

				double logFactor = 1 / ((x - stochast->Shift) * sigma * sqrt(2 * std::numbers::pi));
				double logDistance = log(x - stochast->Shift) - mu;
				double logDistancePower = -(logDistance * logDistance) / (2 * sigma * sigma);

				return logFactor * exp(logDistancePower);
			}
		}

		double LogNormalDistribution::getCDF(StochastProperties* stochast, double x)
		{
			if (x <= stochast->Shift)
			{
				return 0;
			}
			else
			{
				double u = this->getUFromX(stochast, x);

				return StandardNormal::getPFromU(u);
			}
		}

		void LogNormalDistribution::setXAtU(StochastProperties* stochast, double x, double u, ConstantParameterType constantType)
		{
			if (stochast->Scale <= 0)
			{
				setMeanAndDeviation(stochast, x, 0);
			}
			else if (x <= stochast->Shift)
			{
				setMeanAndDeviation(stochast, x, 0);
			}
			else
			{
				return this->setXAtUByIteration(stochast, x, u, constantType);
			}
		}

		double LogNormalDistribution::fitShift(std::vector<double> values)
		{
			// see https://stats.stackexchange.com/questions/49495/robust-parameter-estimation-for-shifted-log-normal-distribution

			double minX = *std::min_element(values.begin(), values.end());

			double min = minX - Numeric::NumericSupport::getFraction(minX, 1);
			double max = minX - Numeric::NumericSupport::getFraction(minX, 1E-6);

			std::unique_ptr<Numeric::BisectionRootFinder> bisection = std::make_unique<Numeric::BisectionRootFinder>();

			Numeric::RootFinderMethod method = [this, &values](double shift)
			{
				int n = values.size();
				int n3 = n / 3;
				double mid = getPartialAverage(values, shift, n3 + 1, n - n3);
				double low = getPartialAverage(values, shift, 1, n3);
				double upp = getPartialAverage(values, shift, n - n3 + 1, n);

				double result = (mid - low) - (upp - mid);
				return result;
			};

			double shift = bisection->CalculateValue(min, max, 0, 0.001, method);

			double shiftValue = method(shift);
			if (shiftValue != 0)
			{
				double negShiftValue = method(-shift);
				if (abs((shiftValue - negShiftValue) / shiftValue) < 1E-6)
				{
					shift = 0;
				}
			}

			if (abs(shift) > 1E10)
			{
				shift = min;
			}

			return shift;
		}

		double LogNormalDistribution::getPartialAverage(std::vector<double>& sample, double gamma, int low, int high)
		{
			double sum = 0;
			for (int i = low - 1; i < high; i++)
			{
				sum += std::log(sample[i] - gamma);
			}

			int n = high - low + 1;
			return sum / n;
		}


		void LogNormalDistribution::fit(StochastProperties* stochast, std::vector<double>& values)
		{
			stochast->Shift = fitShift(values);

			std::vector<double> xLog = Numeric::NumericSupport::select(values, [stochast](double v) {return log(v - stochast->Shift); });

			stochast->Location = Numeric::NumericSupport::getMean(xLog);
			stochast->Scale = Numeric::NumericSupport::getStandardDeviation(stochast->Location, xLog);
		}
	}
}


