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
#include <cmath>
#include <functional>
#include <limits>

#include "../../Deltares.Probabilistic/Optimization/GeneticAlgorithm.h"
#include "../../Deltares.Probabilistic/Utils/ProbabilisticLibraryException.h"
#include "../../Deltares.Probabilistic/Optimization/OptimizationProject.h"
#include "../ZModelBuilder.h"

namespace Deltares::Optimization::Test
{
    using Models::ModelSample;
    using Models::ZModel;

    namespace
    {
        GeneticAlgorithmSettings getSettings(size_t dimensions = 2)
        {
            GeneticAlgorithmSettings options;
            options.SearchArea.setDimensions(dimensions);
            for (auto& dimension : options.SearchArea.Dimensions)
            {
                dimension.MinValue = -5.0;
                dimension.MaxValue = 5.0;
            }
            return options;
        }

        std::shared_ptr<ZModel> getModel(const Models::ZLambda& callback)
        {
            auto model = std::make_shared<ZModel>(callback);
            model->setAllowRepository(false);
            model->setUseZFromSample(true);

            return model;
        }
    }

    TEST(GeneticAlgorithmTest, DefaultSettings)
    {
        auto options = getSettings();
        EXPECT_EQ(options.generation_counters.population_count, 60);
        EXPECT_EQ(options.generation_counters.elite_count, 2);
        EXPECT_EQ(options.generation_counters.generation_count, 50);
        EXPECT_DOUBLE_EQ(options.MutationRate, 0.3);
        EXPECT_DOUBLE_EQ(options.CrossOverFractions.getValueByIndex(1), 0.7);
        EXPECT_DOUBLE_EQ(options.MutationFractions.getValueByIndex(1), 0.9);
        GeneticAlgorithm algorithm(options);
        Logging::ValidationReport report;
        algorithm.validate(report);
        EXPECT_TRUE(report.isValid());
        EXPECT_TRUE(algorithm.isValid());
    }

    TEST(GeneticAlgorithmTest, RejectsInvalidSettingsBeforeEvaluatingModel)
    {
        const std::vector<std::function<void(GeneticAlgorithmSettings&)>> invalidate = {
            [](auto& o) { o.generation_counters = GenerationCounters(0, 0, 0, 0); },
            [](auto& o) { o.MutationRate = -0.1; },
            [](auto& o) { o.MutationRate = 1.1; },
            [](auto& o) { o.MutationRate = std::numeric_limits<double>::quiet_NaN(); },
            [](auto& o) { o.CrossOverFractions = ThreeFractions(-0.1, 1.1, 0.0); },
            [](auto& o) { o.CrossOverFractions = ThreeFractions(0.0, 0.5, std::numeric_limits<double>::quiet_NaN()); },
            [](auto& o) { o.MutationFractions = ThreeFractions(-0.1, 1.1, 0.0); },
            [](auto& o) { o.MutationFractions = ThreeFractions(0.0, 0.5,  std::numeric_limits<double>::quiet_NaN()); },
            [](auto& o) { o.MutationCreepReduction = -0.1; },
            [](auto& o) { o.MutationCreepReduction = std::numeric_limits<double>::infinity(); },
            [](auto& o) { o.MutationCreepReduction = std::numeric_limits<double>::quiet_NaN(); },
            [](auto& o) { o.initializePopulationMethod = static_cast<InitializePopulationMethod>(2); },
            [](auto& o) { o.CrossOverRandomFrequency = static_cast<RandomFrequency>(3); },
            [](auto& o) { o.MutationRandomFrequency = static_cast<RandomFrequency>(-1); },
            [](auto& o) { o.CrossOverRandomEffect = static_cast<RandomEffect>(2); },
            [](auto& o) { o.MutationRandomEffect = static_cast<RandomEffect>(-1); },
            [](auto& o) { o.SearchArea.Dimensions.clear(); },
            [](auto& o) { o.SearchArea.Dimensions[0].MaxValue = -5.0; },
            [](auto& o) { o.SearchArea.Dimensions[0].MinValue = std::numeric_limits<double>::quiet_NaN(); },
            [](auto& o) { o.SearchArea.Dimensions[0].MaxValue = std::numeric_limits<double>::infinity(); }
        };
        for (size_t i = 0; i < invalidate.size(); ++i)
        {
            SCOPED_TRACE(i);
            auto options = getSettings();
            invalidate[i](options);
            GeneticAlgorithm algorithm(options);
            Logging::ValidationReport report;
            algorithm.validate(report);
            EXPECT_FALSE(report.isValid());
            EXPECT_FALSE(algorithm.isValid());
            auto z = getModel([](ModelSample& sample) { sample.Z = 0.0; });
            auto model = Models::BaseModelRunner(z);
            EXPECT_THROW(algorithm.getOptimizedSample(model), Reliability::ProbabilisticLibraryException);
            EXPECT_EQ(model.getModelRuns(), 0);
        }
    }

    TEST(GeneticAlgorithmTest, InitializationAndIterationIndices)
    {
        for (auto method : { InitializePopulationMethod::Uniform, InitializePopulationMethod::Parabolic })
        {
            for (size_t dimensions : { 1u, 7u })
            {
                auto options = getSettings(dimensions);
                options.initializePopulationMethod = method;
                options.generation_counters.population_count = 4;
                options.generation_counters.elite_count = 1;
                options.generation_counters.generation_count = 1;
                std::vector<ModelSample> samples;
                auto z = getModel([&](ModelSample& sample)
                {
                    sample.Z = 0.0;
                    samples.push_back(sample);
                });
                auto model = Models::BaseModelRunner(z);
                GeneticAlgorithm algorithm(options);
                const auto result = algorithm.getOptimizedSample(model);
                ASSERT_EQ(samples.size(), 10u);
                EXPECT_EQ(result.modelRuns, 10);
                EXPECT_TRUE(result.succeeded);

                Numeric::RandomValueGenerator rng;
                rng.initialize(true, options.Seed);
                for (int i = 0; i < options.generation_counters.population_count; ++i)
                {
                    EXPECT_EQ(samples[i].IterationIndex, -1);
                    double c = 0.0;
                    double b = 0.0;
                    double a = 0.0;
                    if (method == InitializePopulationMethod::Parabolic)
                    {
                        c = rng.next();
                        b = rng.next();
                        const double distance = std::max(c, 1.0 - c);
                        a = (rng.next() - b) / (distance * distance);
                    }
                    for (size_t gene = 0; gene < dimensions; ++gene)
                    {
                        const double position = dimensions > 1
                            ? static_cast<double>(gene) / static_cast<double>(dimensions - 1) : 0.0;
                        const double ratio = method == InitializePopulationMethod::Uniform
                            ? rng.next() : a * std::pow(position - c, 2) + b;
                        EXPECT_DOUBLE_EQ(samples[i].Values[gene],
                            options.SearchArea.Dimensions[gene].GetAbsoluteValue(ratio));
                    }
                }
                for (size_t i = options.generation_counters.population_count; i < samples.size(); ++i)
                {
                    EXPECT_EQ(samples[i].IterationIndex, 0);
                }
            }
        }
    }

    TEST(GeneticAlgorithmTest, AllMaskModesStayWithinFixedBounds)
    {
        for (int crossover = 0; crossover < 3; ++crossover)
        {
            for (int mutation = 0; mutation < 3; ++mutation)
            {
                SCOPED_TRACE(crossover);
                SCOPED_TRACE(mutation);
                auto options = getSettings(7);
                options.generation_counters.population_count = 10;
                options.generation_counters.generation_count = 3;
                options.MutationRate = 1.0;
                switch(crossover)
                {
                case 0: options.CrossOverFractions = ThreeFractions(1.0, 0.0, 0.0); break;
                case 1: options.CrossOverFractions = ThreeFractions(0.0, 1.0, 0.0); break;
                case 2: options.CrossOverFractions = ThreeFractions(0.0, 0.0, 1.0); break;
                default:throw Reliability::ProbabilisticLibraryException("Invalid cross over value: ", crossover);
                }
                options.MutationFractions = options.CrossOverFractions;
                options.MutationCreepReduction = 2.0;
                for (auto& dimension : options.SearchArea.Dimensions)
                {
                    dimension.Move = false;
                }
                auto z = getModel([](ModelSample& sample)
                {
                    sample.Z = 0.0;
                    for (double value : sample.Values)
                    {
                        EXPECT_TRUE(std::isfinite(value));
                        EXPECT_GE(value, -5.0);
                        EXPECT_LE(value, 5.0);
                        sample.Z += value * value;
                    }
                });
                auto model = Models::BaseModelRunner(z);
                GeneticAlgorithm algorithm(options);
                const auto result = algorithm.getOptimizedSample(model);
                EXPECT_TRUE(result.succeeded);
                EXPECT_EQ(result.values.size(), 7u);
                EXPECT_EQ(result.modelRuns, 58);
                EXPECT_EQ(result.modelRuns, model.getModelRuns());
            }
        }
    }

    TEST(GeneticAlgorithmTest, CrossOverUsesOriginalParents)
    {
        for (int method = 0; method < 3; ++method)
        {
            auto options = getSettings(12);
            options.generation_counters.population_count = 20;
            options.generation_counters.elite_count = 1;
            options.generation_counters.generation_count = 1;
            options.MutationRate = 0.0;
            switch (method)
            {
            case 0: options.CrossOverFractions = ThreeFractions(1.0, 0.0, 0.0); break;
            case 1: options.CrossOverFractions = ThreeFractions(0.0, 1.0, 0.0); break;
            case 2: options.CrossOverFractions = ThreeFractions(0.0, 0.0, 1.0); break;
            default:throw Reliability::ProbabilisticLibraryException("Invalid cross over value: ", method);
            }
            int calls = 0;
            bool mixedParents = false;
            auto z = getModel([&](ModelSample& sample)
            {
                sample.Z = calls;
                if (calls < options.generation_counters.population_count)
                {
                    std::ranges::fill(sample.Values, -5.0 + 0.5 * calls);
                }
                else
                {
                    int transitions = 0;
                    std::vector<double> parents;
                    for (size_t gene = 0; gene < sample.Values.size(); ++gene)
                    {
                        const double value = sample.Values[gene];
                        EXPECT_DOUBLE_EQ((value + 5.0) * 2.0, std::floor((value + 5.0) * 2.0));
                        if (std::ranges::find(parents, value) == parents.end())
                        {
                            parents.push_back(value);
                        }
                        if (gene > 0 && value != sample.Values[gene - 1])
                        {
                            ++transitions;
                        }
                    }
                    EXPECT_LE(parents.size(), 2u);
                    mixedParents = mixedParents || parents.size() == 2;
                    if (method != 0)
                    {
                        EXPECT_LE(transitions, method);
                    }
                }
                ++calls;
            });
            auto model = Models::BaseModelRunner(z);
            GeneticAlgorithm algorithm(options);
            const auto result = algorithm.getOptimizedSample(model);
            EXPECT_TRUE(mixedParents);
            EXPECT_DOUBLE_EQ(result.minimumValue, 0.0);
            EXPECT_EQ(result.values, std::vector<double>(12, -5.0));
        }
    }

    TEST(GeneticAlgorithmTest, CreepAndInverseMutationUseFirstElite)
    {
        for (bool inverse : { false, true })
        {
            auto options = getSettings(1);
            options.generation_counters.population_count = 3;
            options.generation_counters.elite_count = 1;
            options.generation_counters.generation_count = 1;
            options.MutationRate = 1.0;
            options.MutationCreepReduction = 0.0;
            if (inverse)
            {
                options.MutationFractions = ThreeFractions(0.0, 0.0, 1.0);
            }
            else
            {
                options.MutationFractions = ThreeFractions(0.0, 1.0, 0.0);
            }
            int calls = 0;
            auto z = getModel([&](ModelSample& sample)
            {
                sample.Z = calls;
                if (calls < options.generation_counters.population_count)
                {
                    sample.Values[0] = 2.0 + calls;
                }
                else
                {
                    EXPECT_DOUBLE_EQ(sample.Values[0], inverse ? -2.0 : 2.0);
                }
                ++calls;
            });
            auto model = Models::BaseModelRunner(z);
            GeneticAlgorithm algorithm(options);
            const auto result = algorithm.getOptimizedSample(model);
            EXPECT_DOUBLE_EQ(result.values[0], 2.0);
            EXPECT_EQ(calls, 7);
        }
    }

    TEST(GeneticAlgorithmTest, SelectsBetterOfTwoChildren)
    {
        for (bool firstBetter : { false, true })
        {
            auto options = getSettings(1);
            options.generation_counters.population_count = 1;
            options.generation_counters.elite_count = 0;
            options.generation_counters.generation_count = 1;
            options.MutationRate = 1.0;
            options.MutationFractions = ThreeFractions(1.0, 0.0, 0.0);
            std::vector<ModelSample> samples;
            auto z = getModel([&](ModelSample& sample)
            {
                sample.Z = samples.empty() ? -100.0
                    : (samples.size() == 1) == firstBetter ? -1.0 : 1.0;
                samples.push_back(sample);
            });
            auto model = Models::BaseModelRunner(z);
            GeneticAlgorithm algorithm(options);
            const auto result = algorithm.getOptimizedSample(model);
            ASSERT_EQ(samples.size(), 3u);
            EXPECT_DOUBLE_EQ(result.minimumValue, -1.0);
            EXPECT_EQ(result.values, samples[firstBetter ? 1 : 2].Values);
        }
    }

    TEST(GeneticAlgorithmTest, RandomizedMasksAreRepeatableAcrossRuns)
    {
        for (auto frequency : { RandomFrequency::Never, RandomFrequency::Generation, RandomFrequency::Child })
        {
            for (auto effect : { RandomEffect::Free, RandomEffect::Unity })
            {
                auto options = getSettings(7);
                options.generation_counters.population_count = 10;
                options.generation_counters.generation_count = 3;
                options.CrossOverRandomFrequency = frequency;
                options.MutationRandomFrequency = frequency;
                options.CrossOverRandomEffect = effect;
                options.MutationRandomEffect = effect;
                GeneticAlgorithm algorithm(options);
                std::vector<std::vector<double>> previous;
                for (int run = 0; run < 2; ++run)
                {
                    std::vector<std::vector<double>> samples;
                    auto z = getModel([&](ModelSample& sample)
                    {
                        sample.Z = 0.0;
                        for (double value : sample.Values)
                        {
                            EXPECT_GE(value, -5.0);
                            EXPECT_LE(value, 5.0);
                            sample.Z += value * value;
                        }
                        samples.push_back(sample.Values);
                    });
                    auto model = Models::BaseModelRunner(z);
                    const auto result = algorithm.getOptimizedSample(model);
                    EXPECT_TRUE(result.succeeded);
                    EXPECT_EQ(result.modelRuns, 58);
                    if (run > 0)
                    {
                        EXPECT_EQ(samples, previous);
                    }
                    previous = std::move(samples);
                }
            }
        }
    }

    TEST(GeneticAlgorithmTest, MinimizesQuadratic)
    {
        auto options = getSettings();
        options.generation_counters.population_count = 100;
        options.generation_counters.generation_count = 100;
        auto z = getModel([](ModelSample& sample)
        {
            sample.Z = std::pow(sample.Values[0] - 1.5, 2) + std::pow(sample.Values[1] + 2.0, 2);
        });
        auto model = Models::BaseModelRunner(z);
        GeneticAlgorithm algorithm(options);
        const auto result = algorithm.getOptimizedSample(model);
        ASSERT_EQ(result.values.size(), 2u);
        EXPECT_TRUE(result.succeeded);
        EXPECT_NEAR(result.values[0], 1.5, 0.1);
        EXPECT_NEAR(result.values[1], -2.0, 0.1);
        EXPECT_LT(result.minimumValue, 0.01);
        EXPECT_EQ(result.modelRuns, 19700);
    }

    TEST(GeneticAlgorithmTest, CancellationReturnsCurrentBestWithoutSuccess)
    {
        auto options = getSettings();
        options.generation_counters.population_count = 4;
        options.generation_counters.elite_count = 1;
        options.generation_counters.generation_count = 5;
        GeneticAlgorithm algorithm(options);
        int calls = 0;
        auto z = getModel([&](ModelSample& sample)
        {
            sample.Z = calls++;
            if (sample.IterationIndex == 0)
            {
                algorithm.Stop();
            }
        });
        auto model = Models::BaseModelRunner(z);
        const auto result = algorithm.getOptimizedSample(model);
        EXPECT_FALSE(result.succeeded);
        EXPECT_DOUBLE_EQ(result.minimumValue, 0.0);
        EXPECT_EQ(result.modelRuns, 10);
        EXPECT_EQ(result.modelRuns, calls);
    }

    TEST(GeneticAlgorithmTest, test_project_polynome_move_grid)
    {
        auto project = OptimizationProject();
        project.settings->OptimizationMethod = OptimizationMethodType::GA;
        project.model = Probabilistic::Test::ZModelBuilder::getUnequalPolynomeModel(12.9, 16.2);
        auto searchArea = project.settings->SearchArea;
        searchArea->setDimensions(2);
        searchArea->Dimensions[0]->MinValue = 0;
        searchArea->Dimensions[0]->MaxValue = 10;
        searchArea->Dimensions[0]->Move = true;
        searchArea->Dimensions[1]->MinValue = 0;
        searchArea->Dimensions[1]->MaxValue = 10;
        searchArea->Dimensions[1]->Move = true;

        Logging::ValidationReport report;
        project.settings->validate(report);
        ASSERT_TRUE(report.isValid()) << "validation of settings fails";

        project.run();
        auto result = project.result;
        EXPECT_NEAR(result->values[0], 12.9, 0.001);
        EXPECT_NEAR(result->values[1], 16.2, 0.001);
        EXPECT_NEAR(result->minimumValue, 0.0, 0.001);

        EXPECT_EQ(result->modelRuns, 11825); // reused runs
        EXPECT_TRUE(result->succeeded);
    }

}
