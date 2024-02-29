#pragma once

#include "Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		class HistogramDistribution : public Distribution
		{
		public:
			void initializeForRun(std::shared_ptr<StochastProperties> stochast) override;
			double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
			double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
			bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
			bool isValid(std::shared_ptr<StochastProperties> stochast) override;
			bool canFit() override { return true; }
			double getMean(std::shared_ptr<StochastProperties> stochast) override;
			double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
			void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
			double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
			double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
			void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values) override;
		private:
			double getSizeForEmptySizedRange(std::shared_ptr<StochastProperties> stochast);
		};
	}
}

