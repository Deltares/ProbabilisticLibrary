#pragma once
#include <functional>

#include "../StochastProperties.h"

namespace Deltares
{
	namespace Statistics
	{
		enum ConstantParameterType { Deviation, VariationCoefficient };
		typedef std::function<double(double) > UXLambda;

		class Distribution
		{
		public:
			virtual void initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values) {}
			virtual double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) { return 0; }
			virtual double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) { return 0; }
			virtual bool isVarying(std::shared_ptr<StochastProperties> stochast) { return false; }
			virtual bool canTruncate() { return false; }
			virtual bool canInvert() { return false; }
			virtual bool canFit() { return false; }
			virtual bool isShiftUsed() { return false; }
			virtual bool isQualitative() { return false; }
			virtual double getRepresentativeU(std::shared_ptr<StochastProperties> stochast, double u) { return u; }
			virtual double getMean(std::shared_ptr<StochastProperties> stochast) { return 0; }
			virtual double getDeviation(std::shared_ptr<StochastProperties> stochast) { return 0; }
			virtual bool maintainMeanAndDeviation(std::shared_ptr<StochastProperties> stochast) { return true; }
			virtual void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) {}
			virtual void setShift(std::shared_ptr<StochastProperties> stochast, double shift, bool inverted) { stochast->Shift = shift; }
			virtual void initializeForRun(std::shared_ptr<StochastProperties> stochast) {}
			virtual double getPDF(std::shared_ptr<StochastProperties> stochast, double x) { return 0; }
			virtual double getCDF(std::shared_ptr<StochastProperties> stochast, double x) { return 0; }
			virtual void setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType) {}
			virtual void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values) { }
			virtual bool isValid(std::shared_ptr<StochastProperties> stochast) { return true; }
			virtual double getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x);
			virtual std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) { return std::vector<double>(0); }
			virtual std::vector<DistributionPropertyType> getParameters() {	return {}; }
		protected:
			virtual void setXAtUByIteration(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType);
			virtual double getFittedMinimum(std::vector<double>& x);
		};
	}
}


