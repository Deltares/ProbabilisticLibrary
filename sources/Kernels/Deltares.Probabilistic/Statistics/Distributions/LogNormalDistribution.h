#pragma once
#include "Distribution.h"
namespace Deltares
{
	namespace Statistics
	{
		class LogNormalDistribution : public Distribution
		{
		public:
			void initialize(StochastProperties* stochast, std::vector<double> values) override;
			double getXFromU(StochastProperties* stochast, double u) override;
			double getUFromX(StochastProperties* stochast, double x) override;
			bool isVarying(StochastProperties* stochast) override;
			bool canTruncate() override { return true; }
			bool canInvert() override { return true; }
			bool isShiftUsed() override { return true; }
			double getMean(StochastProperties* stochast) override;
			double getDeviation(StochastProperties* stochast) override;
			void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
			void setShift(StochastProperties* stochast, double shift, bool inverted) override;
			double getPDF(StochastProperties* stochast, double x) override;
			double getCDF(StochastProperties* stochast, double x) override;
			void setXAtU(StochastProperties* stochast, double x, double u, ConstantParameterType constantType) override;
			bool canFit() override { return true; }
			void fit(StochastProperties* stochast, std::vector<double>& values) override;
			bool isValid(StochastProperties* stochast) override;
		private:
			double fitShift(std::vector<double> values);
			double getPartialAverage(std::vector<double>& sample, double gamma, int low, int high);

			double requestedMean = nan("");
			double requestedDeviation = nan("");

		};
	}
}

