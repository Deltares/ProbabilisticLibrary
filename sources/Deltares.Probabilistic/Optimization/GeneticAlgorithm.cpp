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

#include "GeneticAlgorithm.h"
#include "../Utils/ProbabilisticLibraryException.h"

#include <algorithm>
#include <cmath>

namespace Deltares::Optimization
{
    using namespace Deltares::Models;

    void GeneticAlgorithm::validate(Logging::ValidationReport& report) const
    {
        Options.validate(report);
    }

    OptimizationResult GeneticAlgorithm::getOptimizedSample(ZModel& model)
    {
        if (!Options.isValid())
        {
            throw Reliability::ProbabilisticLibraryException("Invalid genetic algorithm settings; call validate for details.");
        }

        rng.initialize(true, Options.Seed);
        crossOverMaskRange = { Options.CrossOverScatterFraction,
            Options.CrossOverScatterFraction + Options.CrossOverSinglePointFraction, 1.0 };
        mutationMaskRange = { Options.MutationJumpFraction,
            Options.MutationJumpFraction + Options.MutationCreepFraction, 1.0 };
        crossOverMask.resize(Options.SearchArea.Dimensions.size());
        mutationMask.resize(Options.SearchArea.Dimensions.size());
        auto population = initializePopulation(model);

        for (int generation = 0; generation < Options.GenerationCount && !isStopped(); ++generation)
        {
            if (Options.CrossOverRandomFrequency == RandomFrequency::Generation)
            {
                randomizeMaskRange(crossOverMaskRange, Options.CrossOverRandomEffect);
            }
            if (Options.MutationRandomFrequency == RandomFrequency::Generation)
            {
                randomizeMaskRange(mutationMaskRange, Options.MutationRandomEffect);
            }

            // Preserve the C# elite ordering: mutation is relative to population[0].
            for (int i = Options.EliteCount; i < Options.PopulationCount; ++i)
            {
                for (int elite = 0; elite < Options.EliteCount; ++elite)
                {
                    if (population[i].Z < population[elite].Z)
                    {
                        std::swap(population[i], population[elite]);
                    }
                }
            }

            const auto originalPopulation = population;
            for (int i = Options.EliteCount; i < Options.PopulationCount; ++i)
            {
                auto child1 = createChild(originalPopulation, population[0], model, generation);
                auto child2 = createChild(originalPopulation, population[0], model, generation);
                population[i] = child1.Z < child2.Z ? std::move(child1) : std::move(child2);
            }
        }

        const auto best = std::ranges::min_element(population,
            [](const ModelSample& a, const ModelSample& b) { return a.Z < b.Z; });
        OptimizationResult result;
        result.succeeded = !isStopped();
        result.values = best->Values;
        result.minimumValue = best->Z;
        result.modelRuns = model.getModelRuns();
        return result;
    }

    std::vector<ModelSample> GeneticAlgorithm::initializePopulation(ZModel& model)
    {
        const auto& dimensions = Options.SearchArea.Dimensions;
        std::vector<ModelSample> population;
        population.reserve(Options.PopulationCount);
        for (int i = 0; i < Options.PopulationCount; ++i)
        {
            ModelSample sample(dimensions.size());
            if (Options.InitializePopulationMethod == InitializePopulationMethod::Parabolic)
            {
                // The parabola has the shape f = A (g-c)^2 + B (g normalized).
                // The location c and value B of the extreme are chosen randomly.
                // The value at the farthest side is chosen randomly
                const double c = rng.next();
                const double b = rng.next();
                const double distance = c < 0.5 ? 1.0 - c : c;
                const double a = (rng.next() - b) / (distance * distance);
                for (size_t gene = 0; gene < dimensions.size(); ++gene)
                {
                    const double position = dimensions.size() > 1
                        ? static_cast<double>(gene) / static_cast<double>(dimensions.size() - 1) : 0.0;
                    const double ratio = a * std::pow(position - c, 2) + b;
                    sample.Values[gene] = dimensions[gene].GetAbsoluteValue(ratio);
                }
            }
            else
            {
                for (size_t gene = 0; gene < dimensions.size(); ++gene)
                {
                    sample.Values[gene] = dimensions[gene].GetAbsoluteValue(rng.next());
                }
            }
            model.invoke(sample);
            population.push_back(std::move(sample));
        }
        return population;
    }

    ModelSample GeneticAlgorithm::createChild(const std::vector<ModelSample>& population,
        const ModelSample& mutationAnchor, ZModel& model, int generation)
    {
        const int father = rng.next(Options.PopulationCount);
        const int mother = rng.next(Options.PopulationCount);
        if (Options.CrossOverRandomFrequency == RandomFrequency::Child)
        {
            randomizeMaskRange(crossOverMaskRange, Options.CrossOverRandomEffect);
        }
        if (Options.MutationRandomFrequency == RandomFrequency::Child)
        {
            randomizeMaskRange(mutationMaskRange, Options.MutationRandomEffect);
        }
        determineCrossOverMask();
        determineMutationMask(mutationAnchor);

        ModelSample child(Options.SearchArea.Dimensions.size());
        child.IterationIndex = generation;
        for (size_t gene = 0; gene < child.Values.size(); ++gene)
        {
            child.Values[gene] = population[crossOverMask[gene] == 0 ? father : mother].Values[gene];
            if (rng.next() < Options.MutationRate)
            {
                child.Values[gene] = mutationMask[gene];
            }
        }
        model.invoke(child);
        return child;
    }

    void GeneticAlgorithm::randomizeMaskRange(std::array<double, mask_range_count>& range, RandomEffect effect)
    {
        if (effect == RandomEffect::Free)
        {
            const double first = rng.next();
            const double second = rng.next();
            range = { std::min(first, second), std::max(first, second), 1.0 };
        }
        else
        {
            const int method = rng.next(3);
            range = { method == 0 ? 1.0 : 0.0, method <= 1 ? 1.0 : 0.0, 1.0 };
        }
    }

    int GeneticAlgorithm::selectMask(const std::array<double, mask_range_count>& range)
    {
        const double value = rng.next();
        return value <= range[0] ? 0 : value <= range[1] ? 1 : 2;
    }

    void GeneticAlgorithm::determineCrossOverMask()
    {
        const int method = selectMask(crossOverMaskRange);
        const double count = static_cast<double>(crossOverMask.size());
        if (method == 0)
        {
            for (auto& parent : crossOverMask)
            {
                parent = rng.next() > 0.5 ? 1 : 0;
            }
        }
        else if (method == 1)
        {
            const double point = rng.next() * (count - 2.0) + 0.5;
            for (size_t gene = 0; gene < crossOverMask.size(); ++gene)
            {
                crossOverMask[gene] = static_cast<double>(gene) < point ? 0 : 1;
            }
        }
        else
        {
            double first = rng.next() * (count - 3.0) + 0.5;
            double second = rng.next() * (count - 3.0) + 0.5;
            if (first > second)
            {
                std::swap(first, second);
            }
            second += 1.0;
            for (size_t gene = 0; gene < crossOverMask.size(); ++gene)
            {
                const double position = static_cast<double>(gene);
                crossOverMask[gene] = position < first || position > second ? 0 : 1;
            }
        }
    }

    void GeneticAlgorithm::determineMutationMask(const ModelSample& first)
    {
        const int method = selectMask(mutationMaskRange);
        const auto& dimensions = Options.SearchArea.Dimensions;
        for (size_t gene = 0; gene < dimensions.size(); ++gene)
        {
            const auto& dimension = dimensions[gene];
            if (method == 0)
            {
                mutationMask[gene] = dimension.GetAbsoluteValue(rng.next());
            }
            else if (method == 1)
            {
                mutationMask[gene] = std::clamp(first.Values[gene] + Options.MutationCreepReduction *
                    (2.0 * rng.next() - 1.0) * (dimension.MaxValue - dimension.MinValue),
                    dimension.MinValue, dimension.MaxValue);
            }
            else
            {
                mutationMask[gene] = dimension.MinValue + (dimension.MaxValue - first.Values[gene]);
            }
        }
    }
}
