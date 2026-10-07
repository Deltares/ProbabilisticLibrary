// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//

#pragma once

#include <vector>

#include "OptimizationMethod.h"

#include "AdaptiveParticleSwarmOptimizationSettings.h"

#include "../Math/RandomValueGenerator.h"

namespace Deltares::Optimization
{
    class AdaptiveParticleSwarmOptimization : public OptimizationMethod
    {
    public:
        AdaptiveParticleSwarmOptimizationSettings Options;
        OptimizationResult getOptimizedSample(Models::ZModel& model) override;
    private:
        Models::ModelSample InitializePopulation(Models::ZModel& model, std::vector<Models::ModelSample>& population);
        Numeric::RandomValueGenerator rng;
        void apsoBranch(int generation_index, size_t particle_size, const Models::ModelSample& best_particle,
            Models::ModelSample& population_m);
        void differentialEvolutionBranch(std::vector<Models::ModelSample>& elite, size_t particle_size,
            const Models::ModelSample& best_particle,
            Models::ModelSample& population_m);
    };

}

