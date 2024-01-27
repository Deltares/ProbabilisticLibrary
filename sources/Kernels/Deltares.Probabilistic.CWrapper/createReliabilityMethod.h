#pragma once

#include "basicSettings.h"
#include "../Deltares.Probabilistic.Kernels/Reliability/ReliabilityMethod.h"

class createReliabilityMethod
{
public:
    Deltares::Reliability::ReliabilityMethod* selectMethod(const Deltares::ProbLibCore::basicSettings& bs);

};

