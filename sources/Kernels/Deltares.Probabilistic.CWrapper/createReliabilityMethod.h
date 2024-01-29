#pragma once

#include "basicSettings.h"
#include "../Deltares.Probabilistic.Kernels/Reliability/ReliabilityMethod.h"
#include "../Deltares.Probabilistic.Kernels/Model/RandomSettings.h"

class createReliabilityMethod
{
public:
    Deltares::Reliability::ReliabilityMethod* selectMethod(const Deltares::ProbLibCore::basicSettings& bs);

private:
    RandomSettings* getRnd(const Deltares::ProbLibCore::basicSettings& bs);
};

