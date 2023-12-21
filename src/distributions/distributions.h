#pragma once
#include <vector>
#include "../probFuncs/stochSettings.h"

class distributions
{
public:
    distributions(const distrib & dt);
    double calculateDistribution(const double u) const;
private:
    //const double qMin = 1.0e-300;
    //const double upperLog  = 700.0;
    //const double uLimit = 5.6;
    const distrib & d;
};
