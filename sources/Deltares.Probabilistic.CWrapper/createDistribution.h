#pragma once

#include <memory>

#include "enumDistributions.h"
#include "../Deltares.Probabilistic/Statistics/Stochast.h"

class createDistribution
{
public:
    static std::shared_ptr <Deltares::Statistics::Stochast> createDistribution::createValid(const Deltares::Reliability::EnumDistributions distHR, double p[4]);
private:
    static void create(std::shared_ptr <Deltares::Statistics::Stochast> &s, const Deltares::Reliability::EnumDistributions d, std::vector<double> p);
};

