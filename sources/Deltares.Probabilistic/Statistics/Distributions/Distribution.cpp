#include "Distribution.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include "../../Utils/probLibException.h"

#include <numbers>

namespace Deltares
{
	namespace Statistics
	{
		void Distribution::setXAtUByIteration(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
		{
			const double delta = 0.00001;
			double margin = std::min(delta, std::abs(x / 1000000));

			double currentMean = this->getMean(stochast);
			double currentDeviation = this->getDeviation(stochast);

			std::unique_ptr<Numeric::BisectionRootFinder> bisection = std::make_unique<Numeric::BisectionRootFinder>();

			if (constantType == Deviation)
			{
				Numeric::RootFinderMethod function = [this, stochast, currentDeviation, u](double mean)
				{
					this->setMeanAndDeviation(stochast, mean, currentDeviation);
					return this->getXFromU(stochast, u);
				};

				double newMean = bisection->CalculateValue(x, currentMean, x, margin, function);
				this->setMeanAndDeviation(stochast, newMean, currentDeviation);
			}
			else if (constantType == VariationCoefficient)
			{
				double variationCoefficient = abs(currentDeviation / currentMean);

				Numeric::RootFinderMethod function = [this, stochast, variationCoefficient, u](double mean)
				{
					double deviation = abs(mean * variationCoefficient);
					this->setMeanAndDeviation(stochast, mean, deviation);
					return this->getXFromU(stochast, u);
				};

				double newMean = bisection->CalculateValue(x, currentMean, x, margin, function);

				this->setMeanAndDeviation(stochast, newMean, abs(newMean * variationCoefficient));
			}
			else
			{
				throw Deltares::ProbLibCore::probLibException("Constant type not supported");
			}
		}

		double Distribution::getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x)
		{
			return log(this->getPDF(stochast, x));
		}

		double Distribution::getFittedMinimum(std::vector<double>& x)
		{
			double min = Numeric::NumericSupport::getMinimum(x);
			double max = Numeric::NumericSupport::getMaximum(x);

			double diff = max - min;
			double add = diff / x.size();

			return min - add;
		}
	}
}




