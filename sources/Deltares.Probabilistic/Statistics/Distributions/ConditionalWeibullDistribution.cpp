#include "ConditionalWeibullDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include <cmath>

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
			if (xlog <= 0.0) return 0.0;
			const double xScale = pow(xlog, 1.0 / stochast->Shape);

			return xScale * stochast->Scale;
		}

        double ConditionalWeibullDistribution::getExponent(std::shared_ptr<StochastProperties> stochast, double x)
        {
            x /= stochast->Scale;
            if (x <= 0.0) return -StandardNormal::BetaMax;
            double xlog = pow(x, stochast->Shape);
            double logF = std::pow(stochast->Shift / stochast->Scale, stochast->Shape) - xlog;
            double f = stochast->ShapeB * exp(logF);

            return f;
        }

		double ConditionalWeibullDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
		{
            double f = getExponent(stochast, x);

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
            double pdf = this->getCDF(stochast, x) * getExponent(stochast, x) * stochast->Shape * pow(x / stochast->Scale, stochast->Shape - 1) / stochast->Scale;

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
            double minValue = Numeric::NumericSupport::getMinimum(values);
            double meanValue = Numeric::NumericSupport::getMean(values);
            double estimatedMaxValue = minValue + 2 * (meanValue - minValue);

		    std::shared_ptr<DistributionFitter> fitter = std::make_shared<DistributionFitter>();

            std::vector<double> minValues = { minValue, 0.5, 0.5, 0.5 };
            std::vector<double> maxValues = { estimatedMaxValue, 1.5, 1.5, 1.5 };
            std::vector<double> initValues = { meanValue, 1.0, 1.0, 1.0 };
            std::vector<DistributionPropertyType> properties = { Shift, Scale, Shape, ShapeB };
            std::vector<double> parameters = fitter->fitByLogLikelihood(values, this, stochast, minValues, maxValues, initValues, properties);

            stochast->Shift = std::max(minValue, parameters[0]);
            stochast->Scale = std::max(0.0, parameters[1]);
            stochast->Shape = std::max(0.0, parameters[2]);
            stochast->ShapeB = std::max(0.0, parameters[3]);
        }

		std::vector<double> ConditionalWeibullDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
		{
			throw Deltares::Reliability::probLibException("not implemented");
		}
	}
}



