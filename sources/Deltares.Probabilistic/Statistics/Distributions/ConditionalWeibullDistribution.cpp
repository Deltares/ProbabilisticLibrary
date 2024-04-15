#include "ConditionalWeibullDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/SpecialFunctions.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
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
			setMeanAndDeviation(stochast, values[0], values[1]);
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
			double q = StandardNormal::getQFromU(u);

			if (q == 1)
			{
				return stochast->Shift;
			}
			else 
			{
				const double log = std::log(q);
				const double xlog = std::pow(stochast->Shift / stochast->Scale, stochast->Shape) - log;
				const double xScale = pow(xlog, 1 / stochast->Shape);

				return xScale * stochast->Scale;
			}
		}

		double ConditionalWeibullDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
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

		double ConditionalWeibullDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			if (x <= stochast->Shift)
			{
				return 0;
			}
			else if (stochast->Scale == 0)
			{
				return x == stochast->Shift ? 1 : 0;
			}
			else
			{
				return (stochast->Shape / stochast->Scale) * std::pow( x / stochast->Scale, stochast->Shape - 1) * (1 - getCDF(stochast, x));
			}
		}

		double ConditionalWeibullDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			if (x <= stochast->Shift)
			{
				return 0;
			}
			else if (stochast->Scale == 0)
			{
				return x < stochast->Shape ? 0 : 1;
			}
			else
			{
				return 1 - std::exp(std::pow(stochast->Shift / stochast->Scale, stochast->Shape) - std::pow(x / stochast->Scale, stochast->Shape));
			}
		}

		void ConditionalWeibullDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
		{
			// ignore for the moment
		}

		void ConditionalWeibullDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
		{
			setXAtUByIteration(stochast, x, u, constantType);
		}

		void ConditionalWeibullDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
		{
			stochast->Shift = this->getFittedMinimum(values);

			// fit parameters of a weibull distribution as a start
			std::shared_ptr<Distribution> weibullDistribution = std::make_shared<WeibullDistribution>();

			weibullDistribution->fit(stochast, values);

			std::shared_ptr<DistributionFitter> fitter = std::make_shared<DistributionFitter>();

			std::vector<DistributionPropertyType> properties = { DistributionPropertyType::Shape, DistributionPropertyType::Scale };
			std::vector<double> minValues = { stochast->Shape, stochast->Scale };
			std::vector<double> maxValues = { 2 * stochast->Shape, 2 * stochast->Scale };
			std::vector<double> initValues = { stochast->Shape, stochast->Scale };
			std::vector<double> parameters = fitter->fitByLogLikelihood(values, this, stochast, minValues, maxValues, initValues, properties);

			stochast->Shape = std::max(0.0, parameters[0]);
			stochast->Scale = std::max(0.0, parameters[1]);
		}

		std::vector<double> ConditionalWeibullDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
		{
			std::vector<double> specialPoints{ stochast->Shift };
			return specialPoints;
		}
	}
}



