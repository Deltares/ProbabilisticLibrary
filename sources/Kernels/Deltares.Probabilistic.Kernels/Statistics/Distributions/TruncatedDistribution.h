#pragma once
#include "Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		struct Truncated;

		class TruncatedDistribution : public Distribution
		{
		public:
			TruncatedDistribution(Distribution* innerDistribution)
			{
				this->innerDistribution = innerDistribution;
			}

			double getXFromU(StochastProperties* stochast, double u) override;
			bool isVarying(StochastProperties* stochast) override;
			bool canTruncate() override { return true; };
			bool canInvert() override { return this->innerDistribution->canInvert(); };
			bool isShiftUsed() override { return this->innerDistribution->isShiftUsed(); };
			double getMean(StochastProperties* stochast) override;
			double getDeviation(StochastProperties* stochast) override;
			void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
		private:
			Distribution* innerDistribution = nullptr;

			Truncated GetTruncatedValue(StochastProperties* stochast);
			double GetProbability(StochastProperties* stochast, bool isMinimum);
			double GetUntruncatedU(double u, StochastProperties* stochast);
		};
	}
}

