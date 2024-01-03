#pragma once
#include "StochastProperties.h"

class Distribution
{
public:
	virtual void initialize(StochastProperties* stochast, double* values);

	virtual double getXFromU(StochastProperties* stochast, double u);

	virtual bool isVarying(StochastProperties* stochast);

	virtual double getMean(StochastProperties* stochast);

	virtual double getDeviation(StochastProperties* stochast);

	virtual void setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation);
};

