#pragma once
#include "Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		class DeterministicDistribution : public Distribution
		{
		public:
			void initialize(StochastProperties* stochast, double* values) override;
			double getXFromU(StochastProperties* stochast, double u) override;
			bool isVarying(StochastProperties* stochast) override;
			double getMean(StochastProperties* stochast) override;
			double getDeviation(StochastProperties* stochast) override;
			void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
			double getPDF(StochastProperties* stochast, double x) override;
			double getCDF(StochastProperties* stochast, double x) override;
			void setXAtU(StochastProperties* stochast, double x, double u) override;
			void fit(StochastProperties* stochast, std::vector<double> x) override;
		};
	}
}

