#include "InvertedDistribution.h"
#include "../Stochast.h"
#include "../../Math/NumericSupport.h"

namespace Deltares
{
	namespace Statistics
	{
		double InvertedDistribution::getInvertedValue(StochastProperties* stochast, double value)
		{
			double center = this->innerDistribution->isShiftUsed() ? stochast->Shift : 0;
			return 2 * center - value;
		}

		void InvertedDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
		{
			double invertedMean = this->getInvertedValue(stochast, mean);

			this->innerDistribution->setMeanAndDeviation(stochast, invertedMean, deviation);
		}

		void InvertedDistribution::setShift(StochastProperties* stochast, double shift, bool inverted)
		{
			this->innerDistribution->setShift(stochast, shift, inverted);
		}

		bool InvertedDistribution::canSetMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
		{
			double invertedMean = this->getInvertedValue(stochast, mean);

			return this->innerDistribution->canSetMeanAndDeviation(stochast, invertedMean, deviation);
		}

		bool InvertedDistribution::isVarying(StochastProperties* stochast)
		{
			return this->innerDistribution->isVarying(stochast);
		}

		double InvertedDistribution::getMean(StochastProperties* stochast)
		{
			return this->getInvertedValue(stochast, this->innerDistribution->getMean(stochast));
		}

		double InvertedDistribution::getDeviation(StochastProperties* stochast)
		{
			return this->innerDistribution->getDeviation(stochast);
		}

		double InvertedDistribution::getXFromU(StochastProperties* stochast, double u)
		{
			double xInvert = this->innerDistribution->getXFromU(stochast, -u);

			return this->getInvertedValue(stochast, xInvert);
		}

		double InvertedDistribution::getUFromX(StochastProperties* stochast, double x)
		{
			double xInvert = this->getInvertedValue(stochast, x);

			return - this->innerDistribution->getUFromX(stochast, xInvert);

		}

		double InvertedDistribution::getPDF(StochastProperties* stochast, double x)
		{
			double xInvert = this->getInvertedValue(stochast, x);

			return this->innerDistribution->getPDF(stochast, xInvert);
		}

		double InvertedDistribution::getCDF(StochastProperties* stochast, double x)
		{
			double xInvert = this->getInvertedValue(stochast, x);

			return 1 - this->innerDistribution->getCDF(stochast, xInvert);
		}

		void InvertedDistribution::setXAtU(StochastProperties* stochast, double x, double u, ConstantParameterType constantType)
		{
			double xInvert = this->getInvertedValue(stochast, x);

			this->innerDistribution->setXAtU(stochast, xInvert, -u, constantType);
		}

		void InvertedDistribution::fit(StochastProperties* stochast, std::vector<double>& values)
		{
			std::vector<double> invertedValues = Numeric::NumericSupport::select(values, [this, stochast](double x) {return this->getInvertedValue(stochast, x); });

			this->innerDistribution->fit(stochast, invertedValues);
		}
	}
}

