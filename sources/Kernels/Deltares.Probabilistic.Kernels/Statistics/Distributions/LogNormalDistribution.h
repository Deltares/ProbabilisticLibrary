#pragma once
#include "Distribution.h"
class LogNormalDistribution :    public Distribution
{
	void initialize(StochastProperties* stochast, double* values) override;
	double getXFromU(StochastProperties* stochast, double u) override;
	bool isVarying(StochastProperties* stochast) override;
	bool canTruncate() override { return true; };
	bool canInvert() override { return true; };
	double getMean(StochastProperties* stochast) override;
	double getDeviation(StochastProperties* stochast) override;
	void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) override;
};

