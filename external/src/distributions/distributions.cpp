#include <math.h>
#include "distributions.h"

distributions::distributions(const distrib & dt) : d(dt)
{
}

double distributions::calculateDistribution(const double u) const
{
    return d.getValue(u);
}

