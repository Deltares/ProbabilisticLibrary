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

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include "AdaptiveParticleSwarmOptimization.h"

namespace Deltares::Optimization
{
    using namespace Deltares::Models;

    OptimizationResult AdaptiveParticleSwarmOptimization::getOptimizedSample(ZModel& model)
    {
        rng.initialize(true, Options.Seed);
        auto& search_area = Options.SearchArea;

        std::vector<ModelSample> population;

        auto best_particle = InitializePopulation(model, population);

        const auto particle_size = search_area.Dimensions.size();

        int generation_index = 0;
        int last_improving_generation = 0;

        while (generation_index < Options.GenerationCount)
        {
            std::vector<ModelSample> elite = population;

            std::sort(elite.begin(), elite.end(),
                [](const ModelSample & a, const ModelSample & b)
                {
                    return a.Z < b.Z;
                });

            while (elite.size() > Options.EliteCount)
            {
                elite.pop_back();
            }

            double algorithm_divider =
                std::max(0.1, 0.9 - generation_index / (0.9 * static_cast<double>(Options.GenerationCount)));

            for (int m = 0; m < Options.PopulationCount; ++m)
            {
                if (rng.next() < algorithm_divider)
                {
                    apsoBranch(generation_index, particle_size, best_particle, population[m]);
                }
                else
                {
                    differentialEvolutionBranch(elite, particle_size, best_particle, population[m]);
                }

                model.invoke(population[m]);

                if (population[m].Z < best_particle.Z)
                {
                    best_particle = population[m];

                    last_improving_generation = generation_index;
                }
            }

            //model.ReportResult(bestParticle.Z, generationIndex + 1, Options.GenerationCount);

            if (generation_index -last_improving_generation >= Options.StopAfterNonImprovingGenerations)
            {
                break;
            }

            ++generation_index;
        }

        auto return_value = OptimizationResult();
        return_value.values = best_particle.Values;
        return_value.succeeded = true;
        return_value.minimumValue = best_particle.Z;
        return return_value;
    }

    void AdaptiveParticleSwarmOptimization::apsoBranch(int generation_index, size_t particle_size, const ModelSample& best_particle, ModelSample& population_m)
    {
        auto& search_area = Options.SearchArea;
        const double alpha = std::pow(Options.Delta, generation_index);

        for (size_t n = 0; n < particle_size; ++n)
        {
            if (rng.next() < Options.CrossOver)
            {
                const double random_apso = alpha * ((rng.next() * 2.0) - 1.0);

                const double prev_ratio = search_area.Dimensions[n]->GetRelativeValue(population_m.Values[n]);

                const double best_ratio = search_area.Dimensions[n]->GetRelativeValue(best_particle.Values[n]);

                const double new_ratio = (1.0 - Options.Beta) * prev_ratio + Options.Beta * best_ratio + random_apso;

                if (search_area.Dimensions[n]->Move || (new_ratio >= 0.0 && new_ratio <= 1.0))
                {
                    population_m.Values[n] =
                        search_area.Dimensions[n]->GetAbsoluteValue(new_ratio);
                }
                else
                {
                    population_m.Values[n] = best_particle.Values[n];
                }
            }
        }
    }

    void AdaptiveParticleSwarmOptimization::differentialEvolutionBranch(std::vector<ModelSample>& elite, size_t particle_size, const ModelSample& best_particle, ModelSample& population_m)
    {
        auto& search_area = Options.SearchArea;

        const int maximum = static_cast<int>(elite.size());

        const ModelSample de0 = elite[rng.next(maximum)];

        const ModelSample de1 = elite[rng.next(maximum)];

        const ModelSample de2 = elite[rng.next(maximum)];

        const ModelSample de3 = elite[rng.next(maximum)];

        for (size_t q = 0; q < particle_size; ++q)
        {
            if (rng.next() < Options.CrossOver)
            {
                const double best_ratio = search_area.Dimensions[q]->GetRelativeValue(best_particle.Values[q]);

                const double r0 = search_area.Dimensions[q]->GetRelativeValue(de0.Values[q]);

                const double r1 = search_area.Dimensions[q]->GetRelativeValue(de1.Values[q]);

                const double r2 = search_area.Dimensions[q]->GetRelativeValue(de2.Values[q]);

                const double r3 = search_area.Dimensions[q]->GetRelativeValue(de3.Values[q]);

                const double new_ratio = best_ratio + Options.DifferentialWeight * (r0 - r1 + r2 - r3);

                if (search_area.Dimensions[q]->Move || (new_ratio >= 0.0 && new_ratio <= 1.0))
                {
                    population_m.Values[q] = search_area.Dimensions[q]->GetAbsoluteValue(new_ratio);
                }
                else
                {
                    population_m.Values[q] = best_particle.Values[q];
                }
            }
        }
    }

    ModelSample AdaptiveParticleSwarmOptimization::InitializePopulation(ZModel& model, std::vector<ModelSample>& population)
    {
        auto& search_area = Options.SearchArea;

        const int particle_size =static_cast<int>(search_area.Dimensions.size());

        ModelSample best_particle = ModelSample(particle_size);
        bool first = true;

        for (int k = 0; k < Options.PopulationCount; ++k)
        {
            ModelSample sample(particle_size);

            const double b = rng.next();
            double c = rng.next();

            c = (c < 0.5) ? (1.0 - c) : c;

            const double a = (rng.next() - b) / (c * c);

            for (int genome_index = 0; genome_index < particle_size; ++genome_index)
            {
                const double genome_ratio = particle_size > 1 ? static_cast<double>(genome_index) / (particle_size - 1) : 0.0;

                const double ratio =a * (genome_ratio - c) * (genome_ratio - c) + b;

                sample.Values[genome_index] = search_area.Dimensions[genome_index]->GetAbsoluteValue(ratio);
            }

            model.invoke(sample);

            population.push_back(sample);

            if (first ||sample.Z < best_particle.Z)
            {
                best_particle = sample;
                first = false;
            }

            //model.ReportResult(bestParticle.Z, 0, Options.GenerationCount);
        }

        return best_particle;
    }
};

