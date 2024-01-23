#pragma once
#include "Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		class InvertedDistribution : public Distribution
		{
		public:

			InvertedDistribution(Distribution* innerDistribution)
			{
				this->innerDistribution = innerDistribution;
			}

			double getXFromU(StochastProperties* stochast, double u) override;
			double getUFromX(StochastProperties* stochast, double x) override;
			bool isVarying(StochastProperties* stochast) override;
			bool canTruncate() override { return this->innerDistribution->canTruncate(); };
			bool canInvert() override { return true; };
			double getMean(StochastProperties* stochast) override;
			double getDeviation(StochastProperties* stochast) override;
			void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
		private:
			Distribution* innerDistribution = nullptr;
			StochastProperties* getInvertedStochast(StochastProperties* stochast);
			double getInvertedValue(StochastProperties* stochast, double value);
		};
	}
}
