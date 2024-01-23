#pragma once
#include "Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		class UniformDistribution : public Distribution
		{
			void initialize(StochastProperties* stochast, double* values) override;
			double getXFromU(StochastProperties* stochast, double u) override;
			double getUFromX(StochastProperties* stochast, double x) override;
			bool isVarying(StochastProperties* stochast) override;
			double getMean(StochastProperties* stochast) override;
			double getDeviation(StochastProperties* stochast) override;
			void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
		};
	}
}

