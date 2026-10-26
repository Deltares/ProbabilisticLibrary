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

#pragma once

#include <array>
#include <utility>
#include <vector>

#include "GeneticAlgorithmSettings.h"
#include "OptimizationMethod.h"
#include "../Math/RandomValueGenerator.h"

namespace Deltares::Optimization
{
    namespace Test
    {
        class GeneticAlgorithmOriginalTest;
    }

    /**
     * \brief Genetic optimization with elitism and selection of the better of two children.
     * \remark Uses the kernel random generator, not the C# System.Random sequence.
     * Search bounds remain fixed, including when SearchParameterSettings::Move is set.
     */
    class GeneticAlgorithm : public OptimizationMethod
    {
    public:
        GeneticAlgorithm() = default;
        explicit GeneticAlgorithm(GeneticAlgorithmSettings options) : Options(std::move(options)) {}

        OptimizationResult getOptimizedSample(Models::ZModel& model) override;
        void validate(Logging::ValidationReport& report) const;
        bool isValid() override { return Options.isValid(); }

    private:
        friend class Test::GeneticAlgorithmOriginalTest;
        static constexpr int mask_range_count = 3;

        GeneticAlgorithmSettings Options;
        Numeric::RandomValueGenerator rng;
        std::array<double, mask_range_count> crossOverMaskRange{};
        std::array<double, mask_range_count> mutationMaskRange{};
        std::vector<int> crossOverMask;
        std::vector<double> mutationMask;

        std::vector<Models::ModelSample> initializePopulation(Models::ZModel& model);
        Models::ModelSample createChild(const std::vector<Models::ModelSample>& population,
            const Models::ModelSample& mutationAnchor, Models::ZModel& model, int generation);
        void randomizeMaskRange(std::array<double, mask_range_count>& range, RandomEffect effect);
        void determineCrossOverMask();
        void determineMutationMask(const Models::ModelSample& first);
        int selectMask(const std::array<double, mask_range_count>& range);
    };
}
