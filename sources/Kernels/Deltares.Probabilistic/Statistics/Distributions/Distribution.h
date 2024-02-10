#pragma once
#include "../StochastProperties.h"

namespace Deltares
{
	namespace Statistics
	{
		enum ConstantParameterType { Deviation, VariationCoefficient };

		class Distribution
		{
		public:
			virtual void initialize(StochastProperties* stochast, std::vector<double> values) {}
			virtual double getXFromU(StochastProperties* stochast, double u) { return 0; }
			virtual double getUFromX(StochastProperties* stochast, double x) { return 0; }
			virtual bool isVarying(StochastProperties* stochast) { return false; }
			virtual bool canTruncate() { return false; }
			virtual bool canInvert() { return false; }
			virtual bool canFit() { return false; }
			virtual bool isShiftUsed() { return false; }
			virtual bool isQualitative() { return false; }
			virtual double getRepresentativeU(StochastProperties* stochast, double u) { return u; }
			virtual double getMean(StochastProperties* stochast) { return 0; }
			virtual double getDeviation(StochastProperties* stochast) { return 0; }
			virtual void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) {}
			virtual void setShift(StochastProperties* stochast, double shift, bool inverted) { stochast->Shift = shift; }
			virtual void initializeForRun(StochastProperties* stochast) {}
			virtual double getPDF(StochastProperties* stochast, double x) { return 0; }
			virtual double getCDF(StochastProperties* stochast, double x) { return 0; }
			virtual void setXAtU(StochastProperties* stochast, double x, double u, ConstantParameterType constantType) {}
			virtual void fit(StochastProperties* stochast, std::vector<double>& values) { }
		protected:
			virtual void setXAtUByIteration(StochastProperties* stochast, double x, double u, ConstantParameterType constantType);
		};
	}
}


