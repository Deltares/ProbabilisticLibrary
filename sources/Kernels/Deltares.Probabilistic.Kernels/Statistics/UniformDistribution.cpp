#include "pch.h"
#include "UniformDistribution.h"
#include "StandardNormal.h"
#include <cmath>

void UniformDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
{
	double diff = sqrt(3) * deviation;
	stochast->Minimum = mean - diff;
	stochast->Maximum = mean + diff;
}

void UniformDistribution::initialize(StochastProperties* stochast, double* values)
{
	stochast->Minimum = values[0];
	stochast->Maximum = values[1];
}

bool UniformDistribution::isVarying(StochastProperties* stochast)
{
	return stochast->Maximum > stochast->Minimum;
}

double UniformDistribution::getMean(StochastProperties* stochast)
{
	return (stochast->Minimum + stochast->Maximum) / 2;
}

double UniformDistribution::getDeviation(StochastProperties* stochast)
{
	double diff = stochast->Maximum - stochast->Minimum;
	return sqrt(diff * diff / 12);
}

double UniformDistribution::getXFromU(StochastProperties* stochast, double u)
{
	double p = StandardNormal::getPFromU(u);

	return stochast->Minimum + p * (stochast->Maximum - stochast->Minimum);
}


