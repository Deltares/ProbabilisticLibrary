#pragma once

#include "../Model/RandomSettings.h"
#include "DesignPointBuilder.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
    namespace Reliability
    {
        class CrudeMonteCarloSettings
        {
        public:
            int MinimumSamples = 1000;
            int MaximumSamples = 10000;
            double VariationCoefficient = 0.05;
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;
            std::shared_ptr<Deltares::Models::RandomSettings> randomSettings = std::make_shared<Deltares::Models::RandomSettings>();
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();
        };
    }
}

