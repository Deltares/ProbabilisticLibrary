#pragma once
#include "Distribution.h"
namespace Deltares
{
	namespace Statistics
	{
		class DiscreteDistribution : public Distribution
		{
			double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
			double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
			bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
			double getMean(std::shared_ptr<StochastProperties> stochast) override;
			double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
			bool maintainMeanAndDeviation(std::shared_ptr<StochastProperties> stochast) override { return false; }
			void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
			void initializeForRun(std::shared_ptr<StochastProperties> stochast) override;
			double getRepresentativeU(std::shared_ptr<StochastProperties> stochast, double u) override;
			double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
			double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool canFit() override { return true; }
			void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values) override;
            void fitWeighted(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, std::vector<double>& weights) override;
			bool isValid(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
		};
	}
}

