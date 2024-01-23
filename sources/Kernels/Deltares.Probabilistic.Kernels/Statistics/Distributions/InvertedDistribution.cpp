#include "InvertedDistribution.h"
#include "../Stochast.h"

namespace Deltares
{
	namespace Statistics
	{
		double InvertedDistribution::getInvertedValue(StochastProperties* stochast, double value)
		{
			double center = this->innerDistribution->isShiftUsed() ? stochast->Shift : 0;
			return 2 * center - value;
		}

		StochastProperties* InvertedDistribution::getInvertedStochast(StochastProperties* stochast)
		{
			StochastProperties* invertedStochast = new StochastProperties();

			invertedStochast->Location = getInvertedValue(stochast, stochast->Location);
			invertedStochast->Minimum = getInvertedValue(stochast, stochast->Minimum);
			invertedStochast->Maximum = getInvertedValue(stochast, stochast->Maximum);
			invertedStochast->Shift = getInvertedValue(stochast, stochast->Shift);
			invertedStochast->ShiftB = stochast->ShiftB;
			invertedStochast->Shape = stochast->Shape;
			invertedStochast->ShapeB = stochast->ShapeB;
			invertedStochast->Scale = stochast->Scale;
			invertedStochast->Observations = stochast->Observations;

			innerDistribution->setMeanAndDeviation(invertedStochast, getInvertedValue(stochast, innerDistribution->getMean(stochast)), innerDistribution->getDeviation(stochast));

			return invertedStochast;
		}

		void InvertedDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
		{
			StochastProperties* invertedStochast = getInvertedStochast(stochast);

			setMeanAndDeviation(invertedStochast, this->getInvertedValue(stochast, mean), deviation);
		}

		bool InvertedDistribution::isVarying(StochastProperties* stochast)
		{
			return this->innerDistribution->isVarying(stochast);
		}

		double InvertedDistribution::getMean(StochastProperties* stochast)
		{
			StochastProperties* invertedStochast = getInvertedStochast(stochast);

			return this->getInvertedValue(stochast, this->innerDistribution->getMean(invertedStochast));
		}

		double InvertedDistribution::getDeviation(StochastProperties* stochast)
		{
			StochastProperties* invertedStochast = getInvertedStochast(stochast);

			return this->innerDistribution->getDeviation(invertedStochast);
		}

		double InvertedDistribution::getXFromU(StochastProperties* stochast, double u)
		{
			StochastProperties* invertedStochast = getInvertedStochast(stochast);

			double xInvert = this->innerDistribution->getXFromU(invertedStochast, -u);

			return this->getInvertedValue(stochast, xInvert);
		}

		double InvertedDistribution::getUFromX(StochastProperties* stochast, double x)
		{
			StochastProperties* invertedStochast = getInvertedStochast(stochast);

			double xInvert = this->getInvertedValue(stochast, x);

			return - this->innerDistribution->getUFromX(invertedStochast, xInvert);

		}

	}
}

