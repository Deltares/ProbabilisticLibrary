#pragma once

#include "StartPointCalculatorSettings.h"
#include "StochastSettingsSet.h"
#include "../Model/GradientSettings.h"

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
            double EpsilonBeta = 0.01;

            std::shared_ptr<Models::GradientSettings> GradientSettings = std::make_shared<Models::GradientSettings>();
            std::shared_ptr<StartPointCalculatorSettings> StartPointSettings = std::make_shared<StartPointCalculatorSettings>();
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();
        };
    }
}

