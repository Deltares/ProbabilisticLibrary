#pragma once
#include <memory>

#include "Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		class InvertedDistribution : public Distribution
		{
		public:

			InvertedDistribution(std::shared_ptr<Distribution> innerDistribution)
			{
				this->innerDistribution = innerDistribution;
			}

			double getXFromU(StochastProperties* stochast, double u) override;
			double getUFromX(StochastProperties* stochast, double x) override;
			bool isVarying(StochastProperties* stochast) override;
			bool canTruncate() override { return this->innerDistribution->canTruncate(); }
			bool canInvert() override { return true; }
			bool canFit() override { return this->innerDistribution->canFit(); }
			double getMean(StochastProperties* stochast) override;
			double getDeviation(StochastProperties* stochast) override;
			void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
			void setShift(StochastProperties* stochast, double shift, bool inverted) override;
			double getPDF(StochastProperties* stochast, double x) override;
			double getCDF(StochastProperties* stochast, double x) override;
			void setXAtU(StochastProperties* stochast, double x, double u, ConstantParameterType constantType) override;
			void fit(StochastProperties* stochast, std::vector<double>& values) override;
		private:
			std::shared_ptr<Distribution> innerDistribution = nullptr;
			double getInvertedValue(StochastProperties* stochast, double value);
		};
	}
}
