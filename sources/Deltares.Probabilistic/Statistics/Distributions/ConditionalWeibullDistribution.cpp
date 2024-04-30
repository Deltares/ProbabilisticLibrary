#include "ConditionalWeibullDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include <cmath>
#include <numbers>

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
			return stochast->Scale >= 0 && stochast->Shape >= 0;
		}

		bool ConditionalWeibullDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Scale > 0;
		}

		double ConditionalWeibullDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
		{
			return this->getMeanByIteration(stochast);
		}

		double ConditionalWeibullDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
		{
			return this->getDeviationByIteration(stochast);
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
			if (q <= 3.34e-8)
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
			if (xlog <= 0.0) return 0.0;
			const double xScale = pow(xlog, 1.0 / stochast->Shape);

			return xScale * stochast->Scale;
		}

		double ConditionalWeibullDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
		{
			throw Deltares::Reliability::probLibException("not implemented");
		}

		double ConditionalWeibullDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			throw Deltares::Reliability::probLibException("not implemented");
		}

		double ConditionalWeibullDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			throw Deltares::Reliability::probLibException("not implemented");
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



