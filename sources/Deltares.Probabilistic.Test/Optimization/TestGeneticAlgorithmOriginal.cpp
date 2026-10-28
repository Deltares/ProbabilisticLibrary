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

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <numbers>
#include <random>
#include <vector>

#include "../../Deltares.Probabilistic/Optimization/GeneticAlgorithm.h"

namespace Deltares::Optimization::Test
{
    // Ports GeneticAlgorithmTest.cs; reference draws replace its System.Random constants.
    class GeneticAlgorithmOriginalTest : public ::testing::Test
    {
    protected:
        GeneticAlgorithmSettings options;
        GeneticAlgorithm algorithm;
        std::mt19937_64 referenceGenerator;
        std::uniform_real_distribution<double> referenceDistribution{ 0.0, 1.0 };

        void SetUp() override
        {
            options.PopulationCount = 200;
            options.GenerationCount = 100;
            options.EliteCount = 2;
            options.MutationRate = 0.3;
            referenceGenerator.seed(options.Seed);
        }

        void setBounds(size_t dimensions, double minimum, double maximum)
        {
            options.SearchArea.setDimensions(dimensions);
            for (auto& dimension : options.SearchArea.Dimensions)
            {
                dimension.MinValue = minimum;
                dimension.MaxValue = maximum;
            }
        }

        double nextReference()
        {
            return referenceDistribution(referenceGenerator);
        }

        std::vector<Models::ModelSample> initializePopulation(bool reinitializeRandom = false)
        {
            algorithm.Options = options;
            algorithm.rng.initialize(true, options.Seed);
            algorithm.crossOverMask.resize(options.SearchArea.Dimensions.size());
            algorithm.mutationMask.resize(options.SearchArea.Dimensions.size());
            auto z = std::make_shared<Models::ZModel>([this](Models::ModelSample& sample) { sample.Z = 0.0; });
            z->setAllowRepository(false);
            z->setUseZFromSample(true);
            auto model = Models::BaseModelRunner(z);
            auto population = algorithm.initializePopulation(model);
            if (reinitializeRandom)
            {
                algorithm.rng.restart();
            }
            return population;
        }

        std::array<double, 3> randomCrossOverMask()
        {
            algorithm.randomizeMaskRange(algorithm.crossOverMaskRange, options.CrossOverRandomEffect);
            return algorithm.crossOverMaskRange;
        }

        std::vector<int> determineCrossOverMask()
        {
            algorithm.determineCrossOverMask();
            return algorithm.crossOverMask;
        }

        std::vector<double> determineMutationMask(int method, const Models::ModelSample& anchor)
        {
            algorithm.mutationMaskRange = {
                method == 0 ? 1.0 : 0.0, method <= 1 ? 1.0 : 0.0, 1.0
            };
            algorithm.determineMutationMask(anchor);
            return algorithm.mutationMask;
        }
    };

    TEST_F(GeneticAlgorithmOriginalTest, TestDetermineCrossOverMask)
    {
        setBounds(7, 0.0, 1.0);
        initializePopulation(true);
        const double first = nextReference();
        const double second = nextReference();
        const std::array<double, 3> expectedRange = {
            std::min(first, second), std::max(first, second), 1.0
        };
        const auto range = randomCrossOverMask();
        for (size_t i = 0; i < range.size(); ++i)
        {
            EXPECT_NEAR(range[i], expectedRange[i], 1e-10);
        }

        // Seed 1 selects double-point crossover with the kernel generator.
        const std::vector<int> expectedMask = { 0, 1, 1, 0, 0, 0, 0 };
        EXPECT_EQ(determineCrossOverMask(), expectedMask);
    }

    TEST_F(GeneticAlgorithmOriginalTest, TestDetermineMutationMask)
    {
        setBounds(7, -5.0, 5.0);
        options.initializePopulationMethod = InitializePopulationMethod::Parabolic;
        const auto population = initializePopulation();
        ASSERT_EQ(population.size(), 200u);
        const auto& anchor = population[0];

        const double c = nextReference();
        const double b = nextReference();
        const double distance = std::max(c, 1.0 - c);
        const double a = (nextReference() - b) / (distance * distance);
        std::array<double, 7> expectedAnchor;
        for (size_t gene = 0; gene < expectedAnchor.size(); ++gene)
        {
            const double position = static_cast<double>(gene) / 6.0;
            expectedAnchor[gene] = -5.0 + 10.0 * (a * (position - c) * (position - c) + b);
            EXPECT_NEAR(anchor.Values[gene], expectedAnchor[gene], 1e-10);
        }
        // Parabolic initialization consumes three draws for each population member.
        for (int i = 3; i < options.PopulationCount * 3; ++i)
        {
            nextReference();
        }
        for (int method = 0; method < 3; ++method)
        {
            SCOPED_TRACE(method);
            nextReference(); // Mask-method selection also consumes a draw for a pure mask.
            const auto mask = determineMutationMask(method, anchor);
            ASSERT_EQ(mask.size(), 7u);
            for (size_t gene = 0; gene < mask.size(); ++gene)
            {
                double expected;
                if (method == 0)
                {
                    expected = -5.0 + 10.0 * nextReference();
                }
                else if (method == 1)
                {
                    expected = std::clamp(expectedAnchor[gene] +
                        0.05 * (2.0 * nextReference() - 1.0) * 10.0, -5.0, 5.0);
                }
                else
                {
                    expected = -expectedAnchor[gene];
                }
                EXPECT_NEAR(mask[gene], expected, 1e-10);
            }
        }
    }

    TEST_F(GeneticAlgorithmOriginalTest, TestParabolicGenome)
    {
        setBounds(7, 0.0, 1.0);
        options.initializePopulationMethod = InitializePopulationMethod::Parabolic;
        const auto population = initializePopulation();
        ASSERT_FALSE(population.empty());
        const auto& genome = population[0].Values;
        ASSERT_EQ(genome.size(), 7u);

        const double c = nextReference();
        const double b = nextReference();
        const double endpoint = nextReference();
        const double distance = c < 0.5 ? 1.0 - c : c;
        const double a = (endpoint - b) / (distance * distance);
        // Include the last gene, which the original C# loop omits.
        for (size_t gene = 0; gene < genome.size(); ++gene)
        {
            const double ratio = static_cast<double>(gene) / 6.0;
            const double expected = a * (ratio - c) * (ratio - c) + b;
            EXPECT_NEAR(genome[gene], expected, 1e-10);
        }
    }

    TEST_F(GeneticAlgorithmOriginalTest, TestRastrigin)
    {
        constexpr int drawCount = 100;
        constexpr double tolerance = 2e-3;
        setBounds(2, -5.0, 5.0);
        options.initializePopulationMethod = InitializePopulationMethod::Uniform;
        options.CrossOverRandomFrequency = RandomFrequency::Generation;
        options.MutationRandomFrequency = RandomFrequency::Generation;
        GeneticAlgorithm optimizer(options);
        auto z = std::make_shared<Models::ZModel>([this](Models::ModelSample& sample)
        {
            double fitness = 21.0;
            for (double value : sample.Values)
            {
                fitness += value * value - 10.0 * std::cos(2.0 * std::numbers::pi * value);
            }
            sample.Z = fitness;
        });
        z->setAllowRepository(false);
        z->setUseZFromSample(true);

        double sum = 0.0;
        double sumSquares = 0.0;
        for (int draw = 0; draw < drawCount; ++draw)
        {
            SCOPED_TRACE(draw);
            z->resetModelRuns();
            auto model = Models::BaseModelRunner(z);
            const auto result = optimizer.getOptimizedSample(model);
            ASSERT_TRUE(result.succeeded);
            ASSERT_TRUE(std::isfinite(result.minimumValue));
            const double fittest = result.minimumValue;
            sum += fittest;
            sumSquares += fittest * fittest;
        }
        const double mean = sum / drawCount;
        const double standardDeviation = std::sqrt(std::abs(sumSquares / drawCount - mean * mean));
        EXPECT_NEAR(mean, 1.0, tolerance);
        EXPECT_NEAR(standardDeviation, 0.0, tolerance);
    }
}
