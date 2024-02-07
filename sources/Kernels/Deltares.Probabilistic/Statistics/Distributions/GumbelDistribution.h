#pragma once
#include "Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		class GumbelDistribution : public Distribution
		{
			void initialize(StochastProperties* stochast, double* values) override;
			double getXFromU(StochastProperties* stochast, double u) override;
			double getUFromX(StochastProperties* stochast, double x) override;
			bool isVarying(StochastProperties* stochast) override;
			bool canTruncate() override { return true; }
			bool canInvert() override { return true; }
			bool isShiftUsed() override { return true; }
			double getMean(StochastProperties* stochast) override;
			double getDeviation(StochastProperties* stochast) override;
			void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
			//double getPDF(StochastProperties* stochast, double x) override;
			//double getCDF(StochastProperties* stochast, double x) override;
			//void setXAtU(StochastProperties* stochast, double x, double u) override;
			//void fit(StochastProperties* stochast, std::vector<double> x) override;
		};
	}
}
