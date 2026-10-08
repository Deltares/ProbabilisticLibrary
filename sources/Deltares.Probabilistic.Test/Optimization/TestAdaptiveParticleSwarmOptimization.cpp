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
        auto apso = AdaptiveParticleSwarmOptimization();
        auto search_param_settings = SearchParameterSettings();
        search_param_settings.MinValue = 1;
        search_param_settings.MaxValue = 10;
        search_param_settings.Move = move;
        for (size_t i = 0; i < center.size(); i++)
        {
            apso.Options.SearchArea.Dimensions.push_back(std::make_shared<SearchParameterSettings>(search_param_settings));
        }
        apso.Options.CrossOver = 0.8;
        apso.Options.StopAfterNonImprovingGenerations = 5;
        auto model = Probabilistic::Test::ZmodelWithCenter(center);
        const auto result = apso.getOptimizedSample(model);

        for (size_t i = 0; i < center.size(); i++)
        {
            EXPECT_NEAR(result.values[i], center[i], tolerance);
        }
        EXPECT_LE(model.getEvaluations(), evaluations);
        EXPECT_EQ(model.getEvaluations(), result.modelRuns);
        EXPECT_TRUE(result.succeeded);
    }

    void TestAdaptiveParticleSwarmOptimization::test_project_polynome_move_grid()
    {
        auto project = OptimizationProject();
        project.settings->OptimizationMethod = OptimizationMethodType::APSO;
        project.zModel = Probabilistic::Test::ZModelBuilder::getPolynomeModel(12.9, 16.2);
        project.settings->MaxGridMoves = 10;
        auto searchArea = project.settings->SearchArea;
        searchArea->setDimensions(2);
        searchArea->Dimensions[0]->MinValue = 0;
        searchArea->Dimensions[0]->MaxValue = 10;
        searchArea->Dimensions[0]->NumberOfValues = 11;
        searchArea->Dimensions[0]->Move = true;
        searchArea->Dimensions[0]->NumberOfRefinements = 10;
        searchArea->Dimensions[1]->MinValue = 0;
        searchArea->Dimensions[1]->MaxValue = 10;
        searchArea->Dimensions[1]->NumberOfValues = 11;
        searchArea->Dimensions[1]->Move = true;
        searchArea->Dimensions[1]->NumberOfRefinements = 10;

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

}

