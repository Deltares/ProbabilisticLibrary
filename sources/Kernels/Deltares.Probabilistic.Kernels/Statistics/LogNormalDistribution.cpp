#include "LogNormalDistribution.h"
#include <cmath>

void LogNormalDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
{
	double p = deviation / (mean - stochast->Shift);
	stochast->Scale = sqrt(log(1 + p * p));
	stochast->Location = log(mean - stochast->Shift) - 0.5 * stochast->Scale * stochast->Scale;
}

void LogNormalDistribution::initialize(StochastProperties* stochast, double* values)
{
	stochast->Shift = values[2];
	setMeanAndDeviation(stochast, values[0], values[1]);
}

bool LogNormalDistribution::isVarying(StochastProperties* stochast)
{
	return stochast->Scale > 0;
}

double LogNormalDistribution::getMean(StochastProperties* stochast)
{
	return exp(stochast->Location + 0.5 * stochast->Scale * stochast->Scale) + stochast->Shift;
}

double LogNormalDistribution::getDeviation(StochastProperties* stochast)
{
	double p = sqrt(exp(stochast->Scale * stochast->Scale) - 1);

	double mean = getMean(stochast);

	return p * (mean - stochast->Shift);
}

double LogNormalDistribution::getXFromU(StochastProperties* stochast, double u)
{
	return exp(stochast->Location + u * stochast->Scale) + stochast->Shift;
}

