#pragma once

#include <memory>
#include "StochastSettingsSet.h"

namespace Deltares
{
    namespace Reliability
    {
        class NumericalBisectionSettings
        {
        public:
            int MinimumIterations = 20;

            int MaximumIterations = 50;

            double EpsilonBeta = 0.01;

            /**
             * \brief Settings for individual stochastic variables, such as the start value
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

        };
    }
}
