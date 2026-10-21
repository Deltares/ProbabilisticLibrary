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
    void TestAdaptiveParticleSwarmOptimization::TestBeeswarm(bool move, double tolerance, int evaluations, const std::vector<double>& center)
    {
        AdaptiveParticleSwarmOptimizationSettings options;
        auto search_param_settings = SearchParameterSettings();
        search_param_settings.MinValue = 1;
        search_param_settings.MaxValue = 10;
        search_param_settings.Move = move;
        for (size_t i = 0; i < center.size(); i++)
        {
            options.SearchArea.Dimensions.emplace_back(search_param_settings);
        }
        options.CrossOver = 0.8;
        options.StopAfterNonImprovingGenerations = 5;

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

        EXPECT_EQ(result->modelRuns, 6375); // reused runs
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

}

