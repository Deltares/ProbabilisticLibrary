#pragma once

#include <random>
#include <vector>

#include "OptimizationMethod.h"

#include "AdaptiveParticleSwarmOptimizationSettings.h"

namespace Deltares::Optimization
{
    class AdaptiveParticleSwarmOptimization : OptimizationMethod
    {
    public:
        AdaptiveParticleSwarmOptimizationSettings Options;
        OptimizationResult getOptimizedSampleNew(Models::ZModel& model) const;
        std::shared_ptr<OptimizationResult> getOptimizedSample(Models::ZModel& model) override;
    private:
        Models::ModelSample InitializePopulation(Models::ZModel& model,
            std::vector<Models::ModelSample>& population,
            std::mt19937& rng) const;
    };

}

