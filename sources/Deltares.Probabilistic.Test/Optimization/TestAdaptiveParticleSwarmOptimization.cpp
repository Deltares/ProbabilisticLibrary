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

#include "TestAdaptiveParticleSwarmOptimization.h"

#include <gtest/gtest.h>

#include "../../Deltares.Probabilistic/Optimization/AdaptiveParticleSwarmOptimization.h"
#include "../../Deltares.Probabilistic/Optimization/OptimizationProject.h"
#include "../ZModelBuilder.h"

namespace Deltares::Optimization::Test
{
    void TestAdaptiveParticleSwarmOptimization::TestBeeswarm(bool fixed_lower, bool fixed_upper, double tolerance, int evaluations, const std::vector<double>& center)
    {
        AdaptiveParticleSwarmOptimizationSettings options;
        auto search_param_settings = SearchParameterSettings();
        search_param_settings.MinValue = 1;
        search_param_settings.MaxValue = 10;
        search_param_settings.SetFixedLowerBound(fixed_lower);
        search_param_settings.SetFixedUpperBound(fixed_upper);
        for (size_t i = 0; i < center.size(); i++)
        {
            options.SearchArea.Dimensions.emplace_back(search_param_settings);
        }
        options.CrossOver = 0.8;
        options.generation_counters.stop_after_non_improving_generations = 5;

        auto zModel = Probabilistic::Test::ZModelBuilder::getPolynomeModel(center);
        auto model = Models::BaseModelRunner(zModel);
        auto apso = AdaptiveParticleSwarmOptimization(options);
        const auto result = apso.getOptimizedSample(model);

        for (size_t i = 0; i < center.size(); i++)
        {
            EXPECT_NEAR(result.values[i], center[i], tolerance);
        }
        EXPECT_GT(result.modelRuns, 0);
        EXPECT_LE(result.modelRuns, evaluations);
        EXPECT_TRUE(result.succeeded);
    }

    void TestAdaptiveParticleSwarmOptimization::test_project_polynome_move_grid()
    {
        auto project = OptimizationProject();
        project.settings->OptimizationMethod = OptimizationMethodType::APSO;
        project.model = Probabilistic::Test::ZModelBuilder::getUnequalPolynomeModel(12.9, 16.2);
        auto searchArea = project.settings->SearchArea;
        searchArea->setDimensions(2);
        searchArea->Dimensions[0]->MinValue = 0;
        searchArea->Dimensions[0]->MaxValue = 10;
        searchArea->Dimensions[0]->SetFixedUpperBound(false);
        searchArea->Dimensions[1]->MinValue = 0;
        searchArea->Dimensions[1]->MaxValue = 10;
        searchArea->Dimensions[1]->SetFixedUpperBound(false);

        Logging::ValidationReport report;
        project.settings->validate(report);
        ASSERT_TRUE(report.isValid()) << "validation of settings fails";

        project.run();
        auto result = project.result;
        EXPECT_NEAR(result->values[0], 12.9, 0.001);
        EXPECT_NEAR(result->values[1], 16.2, 0.001);
        EXPECT_NEAR(result->minimumValue, 0.0, 0.001);

        EXPECT_EQ(result->modelRuns, 6375); // reused runs
        EXPECT_TRUE(result->succeeded);
    }

    void TestAdaptiveParticleSwarmOptimization::test_project_polynome_evaluations()
    {
        auto project = OptimizationProject();
        project.settings->OptimizationMethod = OptimizationMethodType::APSO;
        project.model = Probabilistic::Test::ZModelBuilder::getUnequalPolynomeModel(12.9, 16.2);
        project.settings->RunSettings->SaveEvaluations = true;

        auto searchArea = project.settings->SearchArea;
        searchArea->setDimensions(2);
        searchArea->Dimensions[0]->MinValue = 0;
        searchArea->Dimensions[0]->MaxValue = 10;
        searchArea->Dimensions[0]->SetFixedUpperBound(false);
        searchArea->Dimensions[1]->MinValue = 0;
        searchArea->Dimensions[1]->MaxValue = 10;
        searchArea->Dimensions[1]->SetFixedUpperBound(false);

        Logging::ValidationReport report;
        project.settings->validate(report);
        ASSERT_TRUE(report.isValid()) << "validation of settings fails";

        project.run();
        auto result = project.result;

        ASSERT_EQ(result->evaluations.size(), 6375);
        ASSERT_EQ(result->evaluations.front()->Iteration, -1);
        ASSERT_EQ(result->evaluations.back()->Iteration, 49);
        EXPECT_TRUE(result->succeeded);
    }

    void TestAdaptiveParticleSwarmOptimization::TestValidation()
    {
        auto settings = AdaptiveParticleSwarmOptimizationSettings();
        settings.DifferentialWeight = 2.1;
        settings.CrossOver = -1.0;

        Logging::ValidationReport report;

        settings.validate(report);
        EXPECT_FALSE(report.isValid()) << "validation of APSO settings is true, but should be false";
        EXPECT_EQ(2, report.messages.size());
    }

    void TestAdaptiveParticleSwarmOptimization::TestSearchDimensionValidation()
    {
        auto dimensions = SearchParameterSettingsSet();
        dimensions.setDimensions(2);
        dimensions.Dimensions[0].MinValue = 1.0;
        dimensions.Dimensions[0].MaxValue = 1.0;

        Logging::ValidationReport report;

        dimensions.validate(report);
        EXPECT_FALSE(report.isValid()) << "validation of SearchParameterSettingsSet is true, but should be false";
        EXPECT_EQ(1, report.messages.size());
    }

    void TestAdaptiveParticleSwarmOptimization::TestIndicesCandidatesEliteSize1()
    {
        auto settings = AdaptiveParticleSwarmOptimizationSettings();
        settings.generation_counters.elite_count = 1;
        auto apso = AdaptiveParticleSwarmOptimization(settings);

        EXPECT_THROW(apso.getIndicesCandidates(0), Reliability::ProbabilisticLibraryException);
    }

    void TestAdaptiveParticleSwarmOptimization::TestIndicesCandidatesEliteSize2()
    {
        auto settings = AdaptiveParticleSwarmOptimizationSettings();
        settings.generation_counters.elite_count = 2;
        auto apso = AdaptiveParticleSwarmOptimization(settings);

        auto indices = apso.getIndicesCandidates(0);
        EXPECT_EQ(indices.size(), 4);
        EXPECT_EQ(indices[0], 0);
        EXPECT_EQ(indices[1], 1);
        EXPECT_EQ(indices[2], 0);
        EXPECT_EQ(indices[3], 1);
    }

    void TestAdaptiveParticleSwarmOptimization::TestIndicesCandidatesEliteSize3()
    {
        constexpr auto num_iterations = 1000;
        auto settings = AdaptiveParticleSwarmOptimizationSettings();

        auto sum_indices = std::vector<int>(3, 0);
        settings.generation_counters.elite_count = 3;
        auto apso = AdaptiveParticleSwarmOptimization(settings);
        for (int i = 0; i < num_iterations; ++i)
        {
            auto indices = apso.getIndicesCandidates(0);
            EXPECT_EQ(indices.size(), 4);
            EXPECT_EQ(indices[0], 0);
            EXPECT_EQ(indices[1], 1);
            EXPECT_EQ(indices[2], 2);
            EXPECT_TRUE(indices[3] >= 0 && indices[3] < 3);
            sum_indices[indices[3]]++;
        }

        // The expected value of sum_indices is 333.33... , so we check if the sum is within a reasonable range
        constexpr double expected_sum = static_cast<double>(num_iterations) / 3.0;
        constexpr double spread = 10.0; // Allowable spread from the expected value
        for (int i = 0; i < 3; ++i)
        {
            EXPECT_NEAR(sum_indices[i], expected_sum, spread);
        }
    }

    void TestAdaptiveParticleSwarmOptimization::TestIndicesCandidatesEliteSize4orGreater()
    {
        TestIndicesCandidates(4);
        TestIndicesCandidates(8);
    }

    void TestAdaptiveParticleSwarmOptimization::TestIndicesCandidates(const int elite_size)
    {
        constexpr auto num_iterations = 10000;
        auto settings = AdaptiveParticleSwarmOptimizationSettings();

        settings.generation_counters.elite_count = elite_size;
        settings.generation_counters.population_count = 125;
        auto sum_indices = std::vector<int>(settings.generation_counters.elite_count, 0);
        auto apso = AdaptiveParticleSwarmOptimization(settings);
        for (int i = 0; i < num_iterations; ++i)
        {
            const int exclude = i % settings.generation_counters.population_count; // Exclude a different index each iteration
            const auto indices = apso.getIndicesCandidates(exclude);
            EXPECT_EQ(indices.size(), AdaptiveParticleSwarmOptimization::n_terms);
            for (int j = 0; j < AdaptiveParticleSwarmOptimization::n_terms; ++j)
            {
                EXPECT_TRUE(indices[j] >= 0 && indices[j] < elite_size);
                if (elite_size > 4)
                {
                    EXPECT_NE(indices[j], exclude); // Ensure that the excluded index is not in the result
                }
                sum_indices[indices[j]]++;
                for (int k = j + 1; k < AdaptiveParticleSwarmOptimization::n_terms; ++k)
                {
                    EXPECT_NE(indices[j], indices[k]); // Ensure that all indices are unique
                }
            }
        }

        // Each index should be selected approximately: (num_iterations iterations * n_terms indices / elite_size) times
        const double expected_sum = static_cast<double>(num_iterations)
            * static_cast<double>(AdaptiveParticleSwarmOptimization::n_terms)
            / static_cast<double>(elite_size);

        for (int i = 0; i < elite_size; ++i)
        {
            if (elite_size == AdaptiveParticleSwarmOptimization::n_terms)
            {
                EXPECT_EQ(sum_indices[i], num_iterations);
            }
            else
            {
                constexpr double spread = 100.0; // Allowable spread from the expected value
                EXPECT_NEAR(sum_indices[i], expected_sum, spread);
            }
        }
    }
}

