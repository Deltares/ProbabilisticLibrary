#pragma once

#include <memory>

#include "DesignPointBuilder.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
    namespace Reliability
    {
        class NumericalBisectionSettings
        {
        public:
            int MinimumIterations = 8;

            int MaximumIterations = 50;

            double EpsilonBeta = 0.01;

            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

            /**
             * \brief Settings for individual stochastic variables, such as the start value
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            bool isValid() const { return MaximumIterations > 0; }

        };
    }
}
