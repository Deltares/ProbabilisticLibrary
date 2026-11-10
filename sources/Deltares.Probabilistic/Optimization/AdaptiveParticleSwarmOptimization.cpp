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
#include "../Math/ShuffleItems.h"
#include "../Utils/ProbabilisticLibraryException.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <array>

namespace Deltares::Optimization
{
    using namespace Deltares::Models;

    void AdaptiveParticleSwarmOptimization::validate(Logging::ValidationReport& report) const
    {
        Options.validate(report);
    }

    OptimizationResult AdaptiveParticleSwarmOptimization::getOptimizedSample(BaseModelRunner& model)
    {
        rng.initialize(true, Options.Seed);
        const auto& Dimensions = Options.SearchArea.Dimensions;

        std::vector<ModelSample> population;

        auto best_particle = InitializePopulation(model, population);

        const auto particle_size = Dimensions.size();

        int generation_index = 0;
        int last_improving_generation = 0;
        bool succeeded = true;

        while (generation_index < Options.generation_counters.generation_count)
        {
            std::vector<ModelSample> elite = population;

            std::ranges::sort(elite,
                              [](const ModelSample& a, const ModelSample& b)
                              {
                                  return a.Z < b.Z;
                              });

            while (static_cast<int>(elite.size()) > Options.generation_counters.elite_count)
            {
                elite.pop_back();
            }

            const double algorithm_divider =
                std::max(0.1, 0.9 - generation_index / (0.9 * static_cast<double>(Options.generation_counters.generation_count)));

            for (int m = 0; m < Options.generation_counters.population_count; ++m)
            {
                if (Options.generation_counters.elite_count == 1 || rng.next() < algorithm_divider)
                {
                    apsoBranch(generation_index, particle_size, best_particle, population[m]);
                }
                else
                {
                    differentialEvolutionBranch(elite, best_particle, population[m], m);
                }

                population[m].IterationIndex = generation_index;

                model.invoke(population[m]);

                if (population[m].Z < best_particle.Z)
                {
                    best_particle = population[m];

                    last_improving_generation = generation_index;
                }
            }

            model.reportConvergence(best_particle.Z, generation_index + 1, Options.generation_counters.generation_count);

            if (generation_index -last_improving_generation >= Options.generation_counters.stop_after_non_improving_generations)
            {
                break;
            }
            else if (isStopped())
            {
                succeeded = false;
                break;
            }

            ++generation_index;
        }

        auto return_value = createOptimizationResult(model);
        return_value.succeeded = succeeded;
        return_value.values = best_particle.Values;
        return_value.minimumValue = best_particle.Z;
        return return_value;
    }

    bool AdaptiveParticleSwarmOptimization::isValidNewRatio(const SearchParameterSettings& dimension, const double new_ratio)
    {
        const bool check_lower = !dimension.GetFixedLowerBound() || new_ratio >= 0.0;
        const bool check_upper = !dimension.GetFixedUpperBound() || new_ratio <= 1.0;
        return check_lower && check_upper;
    }

    void AdaptiveParticleSwarmOptimization::apsoBranch(int generation_index, size_t particle_size, const ModelSample& best_particle, ModelSample& population_m)
    {
        const auto& Dimensions = Options.SearchArea.Dimensions;
        const double alpha = std::pow(Options.Delta, generation_index);

        for (size_t n = 0; n < particle_size; ++n)
        {
            if (rng.next() < Options.CrossOver)
            {
                const double random_apso = alpha * ((rng.next() * 2.0) - 1.0);

                const double prev_ratio = Dimensions[n].GetRelativeValue(population_m.Values[n]);

                const double best_ratio = Dimensions[n].GetRelativeValue(best_particle.Values[n]);

                const double new_ratio = (1.0 - Options.Beta) * prev_ratio + Options.Beta * best_ratio + random_apso;

                if (isValidNewRatio(Dimensions[n], new_ratio))
                {
                    population_m.Values[n] = Dimensions[n].GetAbsoluteValue(new_ratio);
                }
                else
                {
                    population_m.Values[n] = best_particle.Values[n];
                }
            }
        }
    }

    std::vector<int> AdaptiveParticleSwarmOptimization::getIndicesCandidates(const int exclude)
    {
        const int maximum = Options.generation_counters.elite_count;

        auto items = std::vector<int>();

        switch (maximum)
        {
        case 1:
            throw Reliability::ProbabilisticLibraryException("getIndicesCandidates: elite count cannot be 1");
        case 2:
            {
                for (int i = 0; i < 4; i++)
                {
                    items.push_back(i % 2);
                }
                return items;
            }
            break;
        case 3:
            {
                for (int i = 0; i < 3; i++)
                {
                    items.push_back(i);
                }
                items.push_back(rng.next(3));
                return items;
            }
            break;
        default:
            {
                for (int i = 0; i < maximum; i++)
                {
                    if (maximum == 4 || i != exclude)
                    {
                        items.push_back(i);
                    }
                }
                const auto indices = Numeric::ShuffleItems<int>::GetNShuffledItems(items, rng, n_terms);
                return indices;
            }
            break;
        }
    }

    void AdaptiveParticleSwarmOptimization::differentialEvolutionBranch(const std::vector<ModelSample>& elite,
        const ModelSample& best_particle, ModelSample& population_m, const int m)
    {
        const auto& Dimensions = Options.SearchArea.Dimensions;
        const auto particle_size = Dimensions.size();

        const auto indices = getIndicesCandidates(m);

        for (size_t q = 0; q < particle_size; ++q)
        {
            if (rng.next() < Options.CrossOver)
            {
                const double best_ratio = Dimensions[q].GetRelativeValue(best_particle.Values[q]);

                std::array<double, n_terms> r;
                for (int i = 0; i < n_terms; i++)
                {
                    const auto& sample_de = elite[indices[i]];
                    r[i] = Dimensions[q].GetRelativeValue(sample_de.Values[q]);
                }

                const double new_ratio = best_ratio + Options.DifferentialWeight * (r[0] - r[1] + r[2] - r[3]);

                if (isValidNewRatio(Dimensions[q], new_ratio))
                {
                    population_m.Values[q] = Dimensions[q].GetAbsoluteValue(new_ratio);
                }
                else
                {
                    population_m.Values[q] = best_particle.Values[q];
                }
            }
        }
    }

    ModelSample AdaptiveParticleSwarmOptimization::InitializePopulation(BaseModelRunner& model, std::vector<ModelSample>& population)
    {
        const auto& Dimensions = Options.SearchArea.Dimensions;

        const int particle_size = static_cast<int>(Dimensions.size());

        ModelSample best_particle = ModelSample(particle_size);
        bool first = true;

        for (int k = 0; k < Options.generation_counters.population_count; ++k)
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

                sample.Values[genome_index] = Dimensions[genome_index].GetAbsoluteValue(ratio);
            }

            // initial run
            sample.IterationIndex = -1;

            model.invoke(sample);

            population.push_back(sample);

            if (first || sample.Z < best_particle.Z)
            {
                best_particle = sample;
                first = false;
            }
        }
        model.reportConvergence(best_particle.Z, 0, Options.generation_counters.generation_count);

        return best_particle;
    }
};

