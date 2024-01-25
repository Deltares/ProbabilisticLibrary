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

            Models::GradientSettings* GradientSettings = new Models::GradientSettings();
            StartPointCalculatorSettings* StartPointSettings = new StartPointCalculatorSettings();
            StochastSettingsSet* StochastSet = new StochastSettingsSet();
        };
    }
}

