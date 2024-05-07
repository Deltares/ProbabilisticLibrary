#pragma once
#include <vector>
#include "basicSettings.h"
#include "../Deltares.Probabilistic/Reliability/ReliabilityMethod.h"
#include "../Deltares.Probabilistic/Model/RandomSettings.h"
#include "../Deltares.Probabilistic/Reliability/FORMSettings.h"
#include "../Deltares.Probabilistic/Reliability/DirectionalSamplingSettings.h"

class createReliabilityMethod
{
public:
    Deltares::Reliability::ReliabilityMethod* selectMethod(const Deltares::ProbLibCore::basicSettings& bs, const size_t nStoch, std::vector<std::shared_ptr<Deltares::Statistics::Stochast>>& stochasts);
private:
    Deltares::Models::RandomSettings* getRnd(const Deltares::ProbLibCore::basicSettings& bs);
    std::vector<double> copyStartVector(const double startVector[], const size_t nStoch);
    void fillFormSettings(std::shared_ptr<Deltares::Reliability::FORMSettings>& Settings, const Deltares::ProbLibCore::basicSettings& bs, const size_t nStoch);
    void fillDsSettings(std::shared_ptr<Deltares::Reliability::DirectionalSamplingSettings>& Settings, const Deltares::ProbLibCore::basicSettings& bs);
};

