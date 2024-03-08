#pragma once
#include <vector>
#include "basicSettings.h"
#include "../Deltares.Probabilistic/Reliability/ReliabilityMethod.h"
#include "../Deltares.Probabilistic/Model/RandomSettings.h"
#include "../Deltares.Probabilistic/Reliability/FORMSettings.h"

class createReliabilityMethod
{
public:
    Deltares::Reliability::ReliabilityMethod* selectMethod(const Deltares::ProbLibCore::basicSettings& bs, const size_t nStoch);

private:
    Deltares::Models::RandomSettings* getRnd(const Deltares::ProbLibCore::basicSettings& bs);
    std::vector<double> copyStartVector(const double startVector[], const size_t nStoch);
    void fillFormSettings(std::shared_ptr<Deltares::Reliability::FORMSettings>& Settings, const Deltares::ProbLibCore::basicSettings& bs, const size_t nStoch);
};

