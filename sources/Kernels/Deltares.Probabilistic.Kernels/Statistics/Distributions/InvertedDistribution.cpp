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
			double invertedMean = this->getInvertedValue(stochast, mean);

			this->innerDistribution->setMeanAndDeviation(stochast, invertedMean, deviation);
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

	}
}

