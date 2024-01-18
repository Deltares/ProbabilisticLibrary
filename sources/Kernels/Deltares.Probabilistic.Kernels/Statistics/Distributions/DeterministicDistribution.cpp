#include "DeterministicDistribution.h"
#include "../StochastProperties.h"

bool DeterministicDistribution::isVarying(StochastProperties* stochast)
{
	return false;
}

void DeterministicDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
{
	stochast->Location = mean;
}

void DeterministicDistribution::initialize(StochastProperties* stochast, double* values)
{
	stochast->Location = values[0];
}

double DeterministicDistribution::getMean(StochastProperties* stochast)
{
	return stochast->Location;
}

double DeterministicDistribution::getDeviation(StochastProperties* stochast)
{
	return 0;
}

double DeterministicDistribution::getXFromU(StochastProperties* stochast, double u)
{
    return stochast->Location;
}


