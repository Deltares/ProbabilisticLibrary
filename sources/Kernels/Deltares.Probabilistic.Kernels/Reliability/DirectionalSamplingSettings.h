#pragma once

#include "../Model/RandomSettings.h"
#include "DesignPointBuilder.h"
#include "DirectionReliabilitySettings.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
    namespace Reliability
    {
        class DirectionalSamplingSettings
        {
        public:
            int MinimumSamples = 1000;
            int MaximumSamples = 10000;
            double VariationCoefficient = 0.05;
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;
            DirectionReliabilitySettings* DirectionSettings = new DirectionReliabilitySettings();
            RandomSettings* randomSettings = new ::RandomSettings();
            StochastSettingsSet* StochastSet = new StochastSettingsSet();
            ~DirectionalSamplingSettings()
            {
                delete randomSettings;
                delete StochastSet;
            }
        };
    }
}


