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
#include <gtest/gtest.h>
#include "TestGridSearch.h"
#include "../ZModelBuilder.h"
#include "../../Deltares.Probabilistic/Optimization/GridSearch.h"
#include "../../Deltares.Probabilistic/Optimization/OptimizationProject.h"

using namespace Deltares::Optimization;

namespace Deltares::Probabilistic::Test
{
    void TestGridSearch::allGridSearchTests()
    {
        test_polynome();
        test_polynome_move_grid();
        test_project_polynome_move_grid();
    }

    void TestGridSearch::test_polynome()
    {
        auto gridSearch = GridSearch();
        auto model = ZModelBuilder::getPolynomeModel(2.4, 3.7);
        auto searchArea = gridSearch.Settings.SearchArea;
        searchArea->setDimensions(2);
        searchArea->Dimensions[0]->MinValue = 0;
        searchArea->Dimensions[0]->MaxValue = 10;
        searchArea->Dimensions[0]->NumberOfValues = 11;
        searchArea->Dimensions[1]->MinValue = 0;
        searchArea->Dimensions[1]->MaxValue = 10;
        searchArea->Dimensions[1]->NumberOfValues = 11;

        auto result = gridSearch.getOptimizedSample(model);
        EXPECT_NEAR(result->optimizedSample->Values[0], 2.0, 0.01);
        EXPECT_NEAR(result->optimizedSample->Values[1], 4.0, 0.01);
        EXPECT_NEAR(result->minimumValue, 1.69, 1e-3);

        EXPECT_EQ(result->modelRuns, 121);
        EXPECT_TRUE(result->succeeded);

        // refine
        searchArea->Dimensions[0]->NumberOfRefinements = 3;
        searchArea->Dimensions[1]->NumberOfRefinements = 3;

        auto result2 = gridSearch.getOptimizedSample(model);
        EXPECT_NEAR(result2->optimizedSample->Values[0], 2.4, 0.1);
        EXPECT_NEAR(result2->optimizedSample->Values[1], 3.7, 0.1);
        EXPECT_NEAR(result2->minimumValue, 0.008, 1e-3);

        EXPECT_EQ(result2->modelRuns, 148);
        EXPECT_TRUE(result2->succeeded);
    }

    void TestGridSearch::test_polynome_move_grid()
    {
        auto project = GridSearch();
        auto model = ZModelBuilder::getPolynomeModel(12.9, 16.2);
        project.Settings.MaxGridMoves = 10;
        auto searchArea = project.Settings.SearchArea;
        searchArea->setDimensions(2);
        searchArea->Dimensions[0]->MinValue = 0;
        searchArea->Dimensions[0]->MaxValue = 10;
        searchArea->Dimensions[0]->NumberOfValues = 11;
        searchArea->Dimensions[0]->Move = true;
        searchArea->Dimensions[1]->MinValue = 0;
        searchArea->Dimensions[1]->MaxValue = 10;
        searchArea->Dimensions[1]->NumberOfValues = 11;
        searchArea->Dimensions[1]->Move = true;

        auto result = project.getOptimizedSample(model);
        EXPECT_NEAR(result->optimizedSample->Values[0], 13.0, 0.01);
        EXPECT_NEAR(result->optimizedSample->Values[1], 16.0, 0.01);
        EXPECT_NEAR(result->minimumValue, 0.14, 1e-3);

        EXPECT_EQ(result->modelRuns, 231);
        EXPECT_TRUE(result->succeeded);

        // refine
        searchArea->Dimensions[0]->NumberOfRefinements = 10;
        searchArea->Dimensions[1]->NumberOfRefinements = 10;

        auto result2 = project.getOptimizedSample(model);
        EXPECT_NEAR(result2->optimizedSample->Values[0], 12.9, 0.001);
        EXPECT_NEAR(result2->optimizedSample->Values[1], 16.2, 0.001);
        EXPECT_NEAR(result2->minimumValue, 0.0, 0.001);

        EXPECT_EQ(result2->modelRuns, 112); // reused runs
        EXPECT_TRUE(result2->succeeded);
    }

    void TestGridSearch::test_project_polynome_move_grid()
    {
        auto project = OptimizationProject();
        project.zModel = ZModelBuilder::getPolynomeModel(12.9, 16.2);
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

        project.run();
        auto result = project.result;
        EXPECT_NEAR(result->optimizedSample->Values[0], 12.9, 0.001);
        EXPECT_NEAR(result->optimizedSample->Values[1], 16.2, 0.001);
        EXPECT_NEAR(result->minimumValue, 0.0, 0.001);

        EXPECT_EQ(result->modelRuns, 321); // reused runs
        EXPECT_TRUE(result->succeeded);
    }
}

