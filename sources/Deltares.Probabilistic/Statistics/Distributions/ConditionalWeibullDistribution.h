#pragma once
#include "Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		/// <summary>
		/// Conditional Weibull Distribution described in terms of exceedance
		/// frequencies rather than probabilities.
		/// Not all methods are implemented.
		/// </summary>
		class ConditionalWeibullDistribution : public Distribution
		{
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
			std::vector<DistributionPropertyType> getParameters() override { return { Shift, Scale, Shape }; }
		private:
            double getExponent(std::shared_ptr<StochastProperties> stochast, double x);
			const double tresholdF = 3.34e-8;
		};
	}
}
