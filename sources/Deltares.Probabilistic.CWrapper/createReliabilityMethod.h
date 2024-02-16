#pragma once

#include "basicSettings.h"
#include "../Deltares.Probabilistic/Reliability/ReliabilityMethod.h"
#include "../Deltares.Probabilistic/Model/RandomSettings.h"

class createReliabilityMethod
{
public:
    Deltares::Reliability::ReliabilityMethod* selectMethod(const Deltares::ProbLibCore::basicSettings& bs);

private:
    Deltares::Models::RandomSettings* getRnd(const Deltares::ProbLibCore::basicSettings& bs);
};

