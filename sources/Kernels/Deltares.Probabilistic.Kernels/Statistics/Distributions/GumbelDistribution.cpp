#include "GumbelDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/Constants.h"
#include <cmath>

void GumbelDistribution::setMeanAndDeviation(StochastProperties* stochast, double mean, double deviation)
{
	stochast->Scale = sqrt(6) * deviation / Deltares::Numeric::pi;
	stochast->Shift = mean - stochast->Scale * Deltares::Numeric::eulerMascheroni;
}

void GumbelDistribution::initialize(StochastProperties* stochast, double* values)
{
	setMeanAndDeviation(stochast, values[0], values[1]);
}

bool GumbelDistribution::isVarying(StochastProperties* stochast)
{
	return stochast->Scale > 0;
}

double GumbelDistribution::getMean(StochastProperties* stochast)
{
	return stochast->Shift + stochast->Scale * Deltares::Numeric::eulerMascheroni;
}

double GumbelDistribution::getDeviation(StochastProperties* stochast)
{
	return Deltares::Numeric::pi * stochast->Scale / sqrt(6);
}

double GumbelDistribution::getXFromU(StochastProperties* stochast, double u)
{
	double p = StandardNormal::getPFromU(u);

	if (p == 0)
	{
		return stochast->Shift;
	}
	else
	{
		double logp = -log(p);
		double xscale = -log(logp);
		double x = xscale * stochast->Scale;

		return x + stochast->Shift;
	}
}



