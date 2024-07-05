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
            void fitWeighted(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, std::vector<double>& weights) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
        private:
			double getSizeForEmptySizedRange(std::shared_ptr<StochastProperties> stochast);
            void splitRanges(std::shared_ptr<StochastProperties> stochast, std::vector<std::shared_ptr<WeightedValue>>& values);
            double getAmount(std::shared_ptr<HistogramValue> range, std::vector<std::shared_ptr<WeightedValue>>& values);
            void mergeLowWeights(std::vector<std::shared_ptr<WeightedValue>>& values);
            size_t getDistinctCount(std::vector<std::shared_ptr<WeightedValue>>& values);
		};
	}
}

