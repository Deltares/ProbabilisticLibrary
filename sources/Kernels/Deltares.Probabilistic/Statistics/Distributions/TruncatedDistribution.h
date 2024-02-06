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
			bool canTruncate() override { return true; };
			bool canInvert() override { return this->innerDistribution->canInvert(); };
			bool isShiftUsed() override { return this->innerDistribution->isShiftUsed(); };
			double getMean(StochastProperties* stochast) override;
			double getDeviation(StochastProperties* stochast) override;
			void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
		private:
			std::shared_ptr<Distribution> innerDistribution = nullptr;

			Truncated GetTruncatedValue(StochastProperties* stochast);
			double GetProbability(StochastProperties* stochast, bool isMinimum);
			double GetUntruncatedU(double u, StochastProperties* stochast);
		};
	}
}

