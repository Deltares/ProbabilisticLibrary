#pragma once
#include "StochastProperties.h"

class Distribution
{
public:
	virtual void initialize(StochastProperties* stochast, double* values) {};

	virtual double getXFromU(StochastProperties* stochast, double u) { return 0; };

	virtual bool isVarying(StochastProperties* stochast) { return false; };

	virtual double getMean(StochastProperties* stochast) { return 0; };

	virtual double getDeviation(StochastProperties* stochast) { return 0; };

	virtual void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation) {};
};

