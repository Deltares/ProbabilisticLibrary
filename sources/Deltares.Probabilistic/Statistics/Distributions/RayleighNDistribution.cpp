#include "RayleighNDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/SpecialFunctions.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include <cmath>
#include <numbers>

namespace Deltares
{
	namespace Statistics
	{
		void RayleighNDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
		{
			setMeanAndDeviation(stochast, values[0], values[1]);
		}

		bool RayleighNDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Scale >= 0 && stochast->Shape >= 0;
		}

		bool RayleighNDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Scale > 0 && stochast->Shape > 0;
		}

		std::vector<double> RayleighNDistribution::getValues(std::shared_ptr<StochastProperties> stochast)
		{
			const int steps = 1000;

			std::vector<double> values(steps);

			for (int i = 0; i < steps; i++)
			{
				double p = (i + 0.5) / steps;
				double u = StandardNormal::getUFromP(p);
				double x = this->getXFromU(stochast, u);

				values[i] = x;
			}

			return values;
		}

		double RayleighNDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
		{
			std::vector<double> values = getValues(stochast);
			return Numeric::NumericSupport::getMean(values);
		}

		double RayleighNDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
		{
			std::vector<double> values = getValues(stochast);
			return Numeric::NumericSupport::getStandardDeviation(values);
		}

		double RayleighNDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
		{
			double p; double q;
			StandardNormal::getPQfromU(u, p, q);

			if (stochast->Scale == 0 || stochast->Shape == 0)
			{
				return stochast->Shift;
			}
			else if (p == 0)
			{
				return stochast->Shift;
			}
			else if (q <= 1e-6)
			{
				double qn = std::max(q / stochast->Shape, 1e-300);
				return std::sqrt(-2 * stochast->Scale * stochast->Scale * std::log(qn)) + stochast->Shift;
			}
			else
			{
				double qn = 1 - pow(p, 1 / stochast->Shape);
				return std::sqrt(-2 * stochast->Scale * stochast->Scale * std::log(qn)) + stochast->Shift;
			}
		}

		double RayleighNDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
		{
			if (stochast->Scale == 0 || stochast->Shape == 0)
			{
				return x < stochast->Shift ? -StandardNormal::UMax : StandardNormal::UMax;
			}
			else
			{
				double cdf = getCDF(stochast, x);
				return StandardNormal::getUFromP(cdf);
			}
		}

		double RayleighNDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			x = x - stochast->Shift;

			if (stochast->Scale == 0 || stochast->Shape == 0)
			{
				return x == 0 ? 1 : 0;
			}
			else if (x <= 0)
			{
				return 0;
			}
			else
			{
				const double rayleighExp = std::exp(-x * x / (2 * stochast->Scale * stochast->Scale));

				return (x * stochast->Shape / (stochast->Scale * stochast->Scale)) * pow(1 - rayleighExp, stochast->Shape - 1) * rayleighExp;
			}
		}

		double RayleighNDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			x = x - stochast->Shift;

			if (stochast->Scale == 0 || stochast->Shape == 0)
			{
				return x < 0 ? 0 : 1;
			}
			else if (x <= 0)
			{
				return 0;
			}
			else
			{
				const double rayleighExp = std::exp(-x * x / (2 * stochast->Scale * stochast->Scale));
				return pow(1 - rayleighExp, stochast->Shape);
			}
		}

		void RayleighNDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
		{
			stochast->Scale = deviation / std::sqrt((4 - std::numbers::pi) / 2);
			stochast->Shift = mean - stochast->Scale * std::sqrt(std::numbers::pi / 2);

			if (stochast->Shape != 1.0) 
			{
				std::unique_ptr<Numeric::BisectionRootFinder> bisection = std::make_unique<Numeric::BisectionRootFinder>();

				Distribution* distribution = this;

				Numeric::RootFinderMethod method = [stochast, distribution](double s)
				{
					stochast->Scale = s;
					return distribution->getMean(stochast);
				};

				double minStart = 0.5 * stochast->Scale;
				double maxStart = 1.5 * stochast->Scale;

				stochast->Scale = bisection->CalculateValue(minStart, maxStart, mean, 0.00001, method);
			}
		}

		void RayleighNDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
		{
			this->setXAtUByIteration(stochast, x, u, constantType);
		}

		std::vector<double> RayleighNDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
		{
			std::vector<double> specialPoints{ stochast->Shift };
			return specialPoints;
		}
	}
}



