#pragma once
#include "Distribution.h"
#include "../DistributionType.h"

#include "FrechetDistribution.h"
#include "GumbelDistribution.h"
#include "InvertedDistribution.h"
#include "WeibullDistribution.h"

namespace Deltares
{
	namespace Statistics
	{
		class GeneralizedExtremeValueDistribution : public Distribution
		{
		public:
			void initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values) override;
			bool isValid(std::shared_ptr<StochastProperties> stochast) override;
			double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
			double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
			bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
			bool canTruncate() override { return true; }
			bool canInvert() override { return true; }
			bool isShiftUsed() override { return true; }
			double getMean(std::shared_ptr<StochastProperties> stochast) override;
			double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
			void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
			double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
			double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
			void setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType) override;
			bool canFit() override { return true; }
			void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values) override;
			std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
		private:
			const std::shared_ptr<Distribution> gumbelDistribution = std::make_shared<GumbelDistribution>();
			const std::shared_ptr<Distribution> frechetDistribution = std::make_shared<FrechetDistribution>();
			const std::shared_ptr<Distribution> weibullDistribution = std::make_shared<InvertedDistribution>(std::make_shared<WeibullDistribution>());

			DistributionType getExtremeDistributionType(std::shared_ptr<StochastProperties> stochast);
			std::shared_ptr<Distribution> getDistribution(std::shared_ptr<StochastProperties> stochast);
			std::shared_ptr<StochastProperties> getStochast(std::shared_ptr<StochastProperties> stochast);
			void assign(std::shared_ptr<StochastProperties> source, std::shared_ptr<StochastProperties> target);
		};
	}
}
