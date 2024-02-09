#include "Distribution.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"

#include <numbers>

namespace Deltares
{
	namespace Statistics
	{
		void Distribution::setXAtUByIteration(StochastProperties* stochast, double x, double u, ConstantParameterType constantType)
		{
			const double delta = 0.00001;
			double margin = std::ranges::min(delta, abs(x / 1000000));

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
				throw std::exception("Constant type not supported");
			}
		}
	}
}




