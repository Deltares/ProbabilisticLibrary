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

			double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
			double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
			bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
			bool canTruncate() override { return true; }
			bool canInvert() override { return this->innerDistribution->canInvert(); }
			bool canFit() override { return this->innerDistribution->canFit(); }
			bool isShiftUsed() override { return this->innerDistribution->isShiftUsed(); }
			double getMean(std::shared_ptr<StochastProperties> stochast) override;
			double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
			void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
			void setShift(std::shared_ptr<StochastProperties> stochast, double shift, bool inverted) override;
			double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
			double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
			void setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType) override;
			void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values) override;
			bool isValid(std::shared_ptr<StochastProperties> stochast) override;

		private:
			std::shared_ptr<Distribution> innerDistribution = nullptr;

			Truncated getTruncatedValue(std::shared_ptr<StochastProperties> stochast);
			double getProbability(std::shared_ptr<StochastProperties> stochast, bool isMinimum);
			double getUntruncatedU(double u, std::shared_ptr<StochastProperties> stochast);
		};
	}
}

