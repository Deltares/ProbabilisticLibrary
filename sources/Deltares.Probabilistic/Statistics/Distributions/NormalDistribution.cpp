#include <cmath>
#include "NormalDistribution.h"
#include "../StochastProperties.h"
#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"

#include <numbers>

namespace Deltares
{
	namespace Statistics
	{
		void NormalDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
		{
			stochast->Location = mean;
			stochast->Scale = deviation;
		}

		void NormalDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
		{
			setMeanAndDeviation(stochast, values[0], values[1]);
		}

		bool NormalDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Scale > 0;
		}

		double NormalDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Location;
		}

		double NormalDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
		{
			return stochast->Scale;
		}

		double NormalDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
		{
			return stochast->Location + u * stochast->Scale;
		}

		double NormalDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
		{
			return (x - stochast->Location) / stochast->Scale;
		}

		double NormalDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			double x0 = x - stochast->Location;
			double sigma = stochast->Scale;

			double normalFactor = 1 / (sigma * sqrt(2 * std::numbers::pi));
			double distance = -x0 * x0 / (2 * sigma * sigma);

			return normalFactor * exp(distance);
		}

		double NormalDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
		{
			double u = this->getUFromX(stochast, x);

			return StandardNormal::getPFromU(u);
		}

		void NormalDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
		{
			stochast->Location = x - u * stochast->Scale;
		}

		double NormalDistribution::getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x)
		{
			double x0 = x - stochast->Location;
			double sigma = stochast->Scale;

			double normalFactor = 1 / (sigma * sqrt(2 * std::numbers::pi));
			double distance = -x0 * x0 / (2 * sigma * sigma);

			return log(normalFactor) + distance;
		}

		void NormalDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
		{
			stochast->Location = Numeric::NumericSupport::getMean(values);
			stochast->Scale = Numeric::NumericSupport::getStandardDeviation(stochast->Location, values);
		}
	}
}




