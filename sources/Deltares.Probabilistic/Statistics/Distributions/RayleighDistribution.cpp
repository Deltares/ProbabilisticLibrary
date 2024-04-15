#include "RayleighDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include <cmath>
#include <numbers>

namespace Deltares
{
	namespace Statistics
	{
		void RayleighDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
		{
			setMeanAndDeviation(stochast, values[0], values[1]);
		}

		bool RayleighDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Scale >= 0;
		}

		bool RayleighDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Scale > 0;
		}

		double RayleighDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Shift + stochast->Scale * std::sqrt(std::numbers::pi / 2);
		}

		double RayleighDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Scale * std::sqrt((4 - std::numbers::pi) / 2);
		}

		double RayleighDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
		{
			double q = StandardNormal::getQFromU(u);

			if (q == 1)
			{
				return stochast->Shift;
			}
			else 
			{
				return std::sqrt(-2 * stochast->Scale * stochast->Scale * std::log(q)) + stochast->Shift;
			}
		}

		double RayleighDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
		{
			if (stochast->Scale == 0)
			{
				return x < stochast->Shift ? -StandardNormal::UMax : StandardNormal::UMax;
			}
			else
			{
				double cdf = getCDF(stochast, x);
				return StandardNormal::getUFromP(cdf);
			}
		}

		double RayleighDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			if (stochast->Scale == 0)
			{
				return x == stochast->Shift ? 1 : 0;
			}
			else if (x < stochast->Shift)
			{
				return 0;
			}
			else
			{
				return ((x - stochast->Shift) / (stochast->Scale * stochast->Scale)) * (1 - getCDF(stochast, x));
			}
		}

		double RayleighDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			if (stochast->Scale == 0)
			{
				return x < stochast->Shift ? 0 : 1;
			}
			else if (x < stochast->Shift)
			{
				return 0;
			}
			else
			{
				return 1 - std::exp(-(x - stochast->Shift) * (x - stochast->Shift) / (2 * stochast->Scale * stochast->Scale));
			}
		}

		void RayleighDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
		{
			stochast->Scale = deviation / std::sqrt((4 - std::numbers::pi) / 2);
			stochast->Shift = mean - stochast->Scale * std::sqrt(std::numbers::pi / 2);
		}

		void RayleighDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
		{
			this->setXAtUByIteration(stochast, x, u, constantType);
		}

		void RayleighDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
		{
			double xMin = Numeric::NumericSupport::getMinimum(values);
			double xMax = Numeric::NumericSupport::getMaximum(values);

			stochast->Shift = xMin - (xMax - xMin) / values.size();

			double sum = Numeric::NumericSupport::sum(values, [stochast](double p) {return (p - stochast->Shift) * (p - stochast->Shift); });
			stochast->Scale = std::sqrt( sum/ (2 * values.size()));

			// Unbiased estimator, but cannot handle large sets:
			//stochast.Scale = s * SpecialFunctions.Gamma(x.Length) * Math.Sqrt(x.Length) / SpecialFunctions.Gamma(x.Length + 0.5);
		}

		std::vector<double> RayleighDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
		{
			std::vector<double> specialPoints{ stochast->Shift };
			return specialPoints;
		}
	}
}



