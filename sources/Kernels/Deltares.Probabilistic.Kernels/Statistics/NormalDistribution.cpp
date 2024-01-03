#include "pch.h"
#include "NormalDistribution.h"
#include "StochastProperties.h"

void NormalDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
{
	stochast->Location = mean;
	stochast->Scale = deviation;
}

void NormalDistribution::initialize(StochastProperties* stochast, double* values)
{
	setMeanAndDeviation(stochast, values[0], values[1]);
}

bool NormalDistribution::isVarying(StochastProperties* stochast)
{
	return stochast->Scale > 0;
}

double NormalDistribution::getMean(StochastProperties* stochast)
{
	return stochast->Location;
}

double NormalDistribution::getDeviation(StochastProperties* stochast)
{
	return stochast->Scale;
}

double NormalDistribution::getXFromU(StochastProperties* stochast, double u)
{
	return stochast->Location + u * stochast->Scale;
}


