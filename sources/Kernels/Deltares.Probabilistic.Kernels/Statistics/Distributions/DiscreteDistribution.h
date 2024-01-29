#pragma once
#include "Distribution.h"
namespace Deltares
{
	namespace Statistics
	{
		class DiscreteDistribution : public Distribution
		{
			double getXFromU(StochastProperties* stochast, double u) override;
			double getUFromX(StochastProperties* stochast, double x) override;
			bool isVarying(StochastProperties* stochast) override;
			double getMean(StochastProperties* stochast) override;
			double getDeviation(StochastProperties* stochast) override;
			void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
			void initializeForRun(StochastProperties* stochast) override;
			double getRepresentativeU(StochastProperties* stochast, double u) override;
		};
	}
}

