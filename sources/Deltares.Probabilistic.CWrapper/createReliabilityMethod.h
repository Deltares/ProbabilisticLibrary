#pragma once
#include <vector>
#include "basicSettings.h"
#include "../Deltares.Probabilistic/Reliability/ReliabilityMethod.h"
#include "../Deltares.Probabilistic/Model/RandomSettings.h"
#include "../Deltares.Probabilistic/Reliability/FORMSettings.h"
#include "../Deltares.Probabilistic/Reliability/DirectionalSamplingSettings.h"
#include "../Deltares.Probabilistic/Reliability/ImportanceSamplingSettings.h"

class createReliabilityMethod
{
public:
    std::shared_ptr<Deltares::Reliability::ReliabilityMethod> selectMethod(const Deltares::ProbLibCore::basicSettings& bs,
																		   const size_t nStoch,
																		   std::vector<std::shared_ptr<Deltares::Statistics::Stochast>>& stochasts);

private:
    std::shared_ptr<Deltares::Models::RandomSettings> getRnd(const Deltares::ProbLibCore::basicSettings& bs);
    std::vector<double> copyStartVector(const double startVector[], const size_t nStoch);
    void fillFormSettings(std::shared_ptr<Deltares::Reliability::FORMSettings>& Settings, const Deltares::ProbLibCore::basicSettings& bs, const size_t nStoch);
    void fillDsSettings(std::shared_ptr<Deltares::Reliability::DirectionalSamplingSettings>& Settings, const Deltares::ProbLibCore::basicSettings& bs);
    Deltares::Reliability::DesignPointMethod convertDp(const Deltares::ProbLibCore::DPoptions dp);
    void fillImportanceSamplingSettings(std::shared_ptr<Deltares::Reliability::ImportanceSamplingSettings> settings, const Deltares::ProbLibCore::basicSettings& bs,
        std::vector<std::shared_ptr<Deltares::Statistics::Stochast>>& stochasts);
    void fillStartVector(std::shared_ptr<Deltares::Reliability::StartPointCalculatorSettings> startPoint,
        const Deltares::ProbLibCore::basicSettings& bs, const size_t nStoch);
};

