#pragma once
#include <memory>

#include "Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		struct Truncated;

		class TruncatedDistribution : public Distribution
		{
		public:
			TruncatedDistribution(std::shared_ptr<Distribution> innerDistribution)
			{
				this->innerDistribution = innerDistribution;
			}

			double getXFromU(StochastProperties* stochast, double u) override;
			double getUFromX(StochastProperties* stochast, double x) override;
			bool isVarying(StochastProperties* stochast) override;
			bool canTruncate() override { return true; }
			bool canInvert() override { return this->innerDistribution->canInvert(); }
			bool canFit() override { return this->innerDistribution->canFit(); }
			bool isShiftUsed() override { return this->innerDistribution->isShiftUsed(); }
			double getMean(StochastProperties* stochast) override;
			double getDeviation(StochastProperties* stochast) override;
			void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
			double getPDF(StochastProperties* stochast, double x) override;
			double getCDF(StochastProperties* stochast, double x) override;
			void setXAtU(StochastProperties* stochast, double x, double u, ConstantParameterType constantType) override;
			void fit(StochastProperties* stochast, std::vector<double>& values) override;
		private:
			std::shared_ptr<Distribution> innerDistribution = nullptr;

			Truncated getTruncatedValue(StochastProperties* stochast);
			double getProbability(StochastProperties* stochast, bool isMinimum);
			double getUntruncatedU(double u, StochastProperties* stochast);
		};
	}
}

