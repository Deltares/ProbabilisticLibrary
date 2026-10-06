#pragma once

#include "SearchParameterSettingsSet.h"

namespace Deltares::Optimization
{
    class AdaptiveParticleSwarmOptimizationSettings
    {
    public:
        SearchParameterSettingsSet SearchArea = SearchParameterSettingsSet();
        int Seed = 12345;
        int GenerationCount = 50;
        int EliteCount = 8;
    };
}
