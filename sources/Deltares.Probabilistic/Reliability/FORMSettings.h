#pragma once

#include "StartPointCalculatorSettings.h"
#include "StochastSettingsSet.h"
#include "../Model/GradientSettings.h"
#include "../Model/RunSettings.h"
#include <vector>

namespace Deltares
{
    namespace Reliability
    {
        class FORMSettings
        {
        public:
            double RelaxationFactor = 0.75;
            int RelaxationLoops = 1;
            int MaximumIterations = 50;
            int MaxIterationsGrowthFactor = 1;
            double EpsilonBeta = 0.01;
            bool FilterAtNonConvergence = false;

            std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();
            std::shared_ptr<Models::GradientSettings> GradientSettings = std::make_shared<Models::GradientSettings>();
            std::shared_ptr<StartPointCalculatorSettings> StartPointSettings = std::make_shared<StartPointCalculatorSettings>();
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            bool isValid()
            {
                return RelaxationLoops >= 1 &&
                    RelaxationFactor >= 0.01 &&
                    RelaxationFactor <= 1 &&
                    MaximumIterations >= 1 &&
                    EpsilonBeta >= 0.01 &&
                    GradientSettings->isValid() &&
                    StartPointSettings->isValid() &&
                    RunSettings->isValid();
            }
        };
    }
}

