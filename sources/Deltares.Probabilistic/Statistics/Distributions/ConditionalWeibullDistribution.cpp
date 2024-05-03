#include "ConditionalWeibullDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include <cmath>
#include <cerrno>

#include "DistributionFitter.h"
#include "WeibullDistribution.h"

namespace Deltares
{
	namespace Statistics
	{
		void ConditionalWeibullDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
		{
			throw Deltares::Reliability::probLibException("not implemented");
		}

		bool ConditionalWeibullDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Scale > 0 && stochast->Shape > 0 && stochast->ShapeB > 0;
		}

		bool ConditionalWeibullDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
		{
			return isValid(stochast);
		}

		double ConditionalWeibullDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
		{
			return getMeanByIteration(stochast);
		}

		double ConditionalWeibullDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
		{
			return getDeviationByIteration(stochast);
		}

		double ConditionalWeibullDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
		{
			const double qMin = 1.0e-300;
			double p; double q;
			StandardNormal::getPQfromU(u, p, q);

			if (q == 1.0)
			{
				return stochast->Shift;
			}
			double f; // Exceedance frequency
			if (q <= tresholdF)
			{
				f = std::max(q, qMin);
			}
			else
			{
				double pd = std::max(p, qMin);
				f = -log(pd);
			}

			const double logF = std::log(f / stochast->ShapeB);
			const double xlog = std::pow(stochast->Shift / stochast->Scale, stochast->Shape) - logF;
			errno = 0;
			const double xScale = pow(xlog, 1.0 / stochast->Shape);
			if (errno != 0)
			{
				return 0.0;
			}

			return xScale * stochast->Scale;
		}

		double ConditionalWeibullDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
		{
			x /= stochast->Scale;
			errno = 0;
			double xlog = pow(x, stochast->Shape);
			if (errno != 0)
			{
				return -StandardNormal::BetaMax;
			}
			double logF = std::pow(stochast->Shift / stochast->Scale, stochast->Shape) - xlog;
			double f = stochast->ShapeB * exp(logF);
			double u;
			if (f < tresholdF)
			{
				u = StandardNormal::getUFromQ(f);
			}
			else
			{
				double pd = exp(-f);
				u = StandardNormal::getUFromP(pd);
			}
			return u;
		}

		double ConditionalWeibullDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			const double dx = 1e-4;
			double p2 = getCDF(stochast, x + dx);
			double p1 = getCDF(stochast, x - dx);
			double pdf = (p2 - p1) / (2.0 * dx);
			return pdf;
		}

		double ConditionalWeibullDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			double u = getUFromX(stochast, x);
			return StandardNormal::getPFromU(u);
		}

		void ConditionalWeibullDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
		{
			throw Deltares::Reliability::probLibException("not implemented");
		}

		void ConditionalWeibullDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
		{
			throw Deltares::Reliability::probLibException("not implemented");
		}

		void ConditionalWeibullDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
		{
			throw Deltares::Reliability::probLibException("not implemented");
		}

		std::vector<double> ConditionalWeibullDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
		{
			throw Deltares::Reliability::probLibException("not implemented");
		}
	}
}



