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
#include "TestCobyla.h"
#include "../ZModelBuilder.h"
#include "../../Deltares.Probabilistic/Optimization/CobylaOptimization.h"
#include "../../Deltares.Probabilistic/Optimization/OptimizationProject.h"

using namespace Deltares::Optimization;

namespace Deltares::Probabilistic::Test
{
    void TestCobyla::allCobylaTests()
    {
        test_with_constraint1();
        test_project_no_constraints1();
        test_no_constraints1();
        test_no_constraints2();
    }

    void TestCobyla::test_no_constraints1()
    {
        auto cb = CobylaOptimization();
        auto model = ZModelBuilder::getPolynomeModel();
        auto searchArea = cb.Settings.SearchArea;
        searchArea->setDimensions(2);
        auto result = cb.getOptimizedSample(model);
        EXPECT_NEAR(result.values[0], -1.0, 1e-3);
        EXPECT_NEAR(result.values[1], 0.0, 1e-3);
        EXPECT_NEAR(result.minimumValue, 0.0, 1e-3);
        EXPECT_EQ(result.modelRuns, 69);
        EXPECT_TRUE(result.succeeded);
    }

    void TestCobyla::test_project_no_constraints1()
    {
        auto cb = OptimizationProject();
        cb.zModel = ZModelBuilder::getPolynomeModel();
        cb.settings->OptimizationMethod = OptimizationMethodType::Cobyla;
        auto searchArea = cb.settings->SearchArea;
        searchArea->setDimensions(2);

        cb.run();
        auto result = cb.result;
        EXPECT_NEAR(result->values[0], -1.0, 1e-3);
        EXPECT_NEAR(result->values[1], 0.0, 1e-3);
        EXPECT_NEAR(result->minimumValue, 0.0, 1e-3);
        EXPECT_EQ(result->modelRuns, 69);
        EXPECT_TRUE(result->succeeded);
    }



    void TestCobyla::test_no_constraints2()
    {
        auto cb = CobylaOptimization();
        auto model = ZModelBuilder::getPolynomeModel(2, 3);
        auto searchArea = cb.Settings.SearchArea;
        searchArea->setDimensions(2);
        auto result = cb.getOptimizedSample(model);
        EXPECT_NEAR(result.values[0], 2.0, 1e-3);
        EXPECT_NEAR(result.values[1], 3.0, 1e-3);
        EXPECT_NEAR(result.minimumValue, 0.0, 1e-3);
        EXPECT_EQ(result.modelRuns, 83);
        EXPECT_TRUE(result.succeeded);
    }

    void TestCobyla::test_with_constraint1()
    {
        auto cb = CobylaOptimization();
        auto model = ZModelBuilder::getConstrainedPolynomeModel();
        auto searchArea = cb.Settings.SearchArea;
        searchArea->setDimensions(2);
        searchArea->Dimensions[0]->StartValue = 1.0;
        searchArea->Dimensions[1]->StartValue = 1.0;
        auto result = cb.getOptimizedSample(model);
        EXPECT_NEAR(result.values[0], 0.707, 1e-2);
        EXPECT_NEAR(result.values[1], -0.707, 1e-2);
        EXPECT_NEAR(result.minimumValue, -0.5, 1e-3);
        EXPECT_EQ(result.modelRuns, 49);
        EXPECT_TRUE(result.succeeded);
    }

}

