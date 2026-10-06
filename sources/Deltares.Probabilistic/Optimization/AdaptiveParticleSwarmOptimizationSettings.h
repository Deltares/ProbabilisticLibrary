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
        size_t EliteCount = 8;
        int PopulationCount = 125;
        int StopAfterNonImprovingGenerations = 100;
        double DifferentialWeight = 0.3;
        double CrossOver = 0.3;
        double Beta = 0.5;
        double Delta = 0.7;
    };
}
