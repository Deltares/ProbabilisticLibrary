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

#include "AdaptiveParticleSwarmOptimization.h"
#include "../Math/NumericSupport.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <array>

namespace Deltares::Optimization
{
    using namespace Deltares::Models;

    OptimizationResult AdaptiveParticleSwarmOptimization::getOptimizedSample(ZModel& model)
    {
        rng.initialize(true, Options.Seed);
        const auto& [Dimensions] = Options.SearchArea;

        std::vector<ModelSample> population;

        auto best_particle = InitializePopulation(model, population);
        size_t nr_evaluations = population.size();

        const auto particle_size = Dimensions.size();

        int generation_index = 0;
        int last_improving_generation = 0;

        while (generation_index < Options.GenerationCount)
        {
            std::vector<ModelSample> elite = population;

            std::ranges::sort(elite,
                              [](const ModelSample& a, const ModelSample& b)
                              {
                                  return a.Z < b.Z;
                              });

            while (static_cast<int>(elite.size()) > Options.EliteCount)
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
                nr_evaluations++;

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
        return_value.modelRuns = static_cast<int>(nr_evaluations);
        return return_value;
    }

    void AdaptiveParticleSwarmOptimization::apsoBranch(int generation_index, size_t particle_size, const ModelSample& best_particle, ModelSample& population_m)
    {
        const auto& [Dimensions] = Options.SearchArea;
        const double alpha = std::pow(Options.Delta, generation_index);

        for (size_t n = 0; n < particle_size; ++n)
        {
            if (rng.next() < Options.CrossOver)
            {
                const double random_apso = alpha * ((rng.next() * 2.0) - 1.0);

                const double prev_ratio = Dimensions[n]->GetRelativeValue(population_m.Values[n]);

                const double best_ratio = Dimensions[n]->GetRelativeValue(best_particle.Values[n]);

                const double new_ratio = (1.0 - Options.Beta) * prev_ratio + Options.Beta * best_ratio + random_apso;

                if (Dimensions[n]->Move || (new_ratio >= 0.0 && new_ratio <= 1.0))
                {
                    population_m.Values[n] = Dimensions[n]->GetAbsoluteValue(new_ratio);
                }
                else
                {
                    population_m.Values[n] = best_particle.Values[n];
                }
            }
        }
    }

    void AdaptiveParticleSwarmOptimization::differentialEvolutionBranch(const std::vector<ModelSample>& elite, size_t particle_size, const ModelSample& best_particle, ModelSample& population_m)
    {
        const auto& [Dimensions] = Options.SearchArea;

        const int maximum = static_cast<int>(elite.size());

        constexpr int n_terms = 4;
        std::array<int, n_terms> indexes;
        for (int i = 0 ; i < n_terms; i++)
        {
            indexes[i] = rng.next(maximum);
        }

        for (size_t q = 0; q < particle_size; ++q)
        {
            if (rng.next() < Options.CrossOver)
            {
                const double best_ratio = Dimensions[q]->GetRelativeValue(best_particle.Values[q]);

                std::array<double, n_terms> r;
                for (int i = 0; i < n_terms; i++)
                {
                    const auto& sample_de = elite[indexes[i]];
                    r[i] = Dimensions[q]->GetRelativeValue(sample_de.Values[q]);
                }

                const double new_ratio = best_ratio + Options.DifferentialWeight * (r[0] - r[1] + r[2] - r[3]);

                if (Dimensions[q]->Move || (new_ratio >= 0.0 && new_ratio <= 1.0))
                {
                    population_m.Values[q] = Dimensions[q]->GetAbsoluteValue(new_ratio);
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
        const auto& [Dimensions] = Options.SearchArea;

        const int particle_size = static_cast<int>(Dimensions.size());

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
                const double genome_ratio = particle_size > 1
                    ? Numeric::NumericSupport::Divide (genome_index, particle_size - 1)
                    : 0.0;

                const double ratio = a * std::pow(genome_ratio - c, 2) + b;

                sample.Values[genome_index] = Dimensions[genome_index]->GetAbsoluteValue(ratio);
            }

            model.invoke(sample);

            population.push_back(sample);

            if (first || sample.Z < best_particle.Z)
            {
                best_particle = sample;
                first = false;
            }

            //model.ReportResult(bestParticle.Z, 0, Options.GenerationCount);
        }

        return best_particle;
    }
};

