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

using namespace Deltares::Optimization;

namespace Deltares::Probabilistic::Test
{
    void TestCobyla::allCobylaTests()
    {
        test_with_constraint1();
        test_no_constraints1();
        test_no_constraints2();
    }

    void TestCobyla::test_no_constraints1()
    {
        auto cb = CobylaOptimization();
        auto model = getTestModel();
        auto searchArea = cb.Settings->SearchArea;
        searchArea->setDimensions(2);
        auto result = cb.getOptimizedSample(model);
        EXPECT_NEAR(result->optimizedSample->Values[0], -1.0, 1e-3);
        EXPECT_NEAR(result->optimizedSample->Values[1], 0.0, 1e-3);
        EXPECT_NEAR(result->minimumValue, 0.0, 1e-3);
        EXPECT_EQ(result->totalModelRuns, 69);
        EXPECT_TRUE(result->succeeded);
    }

    void TestCobyla::test_no_constraints2()
    {
        auto cb = CobylaOptimization();
        auto model = getTestModel(2, 3);
        auto searchArea = cb.Settings->SearchArea;
        searchArea->setDimensions(2);
        auto result = cb.getOptimizedSample(model);
        EXPECT_NEAR(result->optimizedSample->Values[0], 2.0, 1e-3);
        EXPECT_NEAR(result->optimizedSample->Values[1], 3.0, 1e-3);
        EXPECT_NEAR(result->minimumValue, 0.0, 1e-3);
        EXPECT_EQ(result->totalModelRuns, 83);
        EXPECT_TRUE(result->succeeded);
    }

    void TestCobyla::test_with_constraint1()
    {
        auto cb = CobylaOptimization();
        auto model = getTestModelWithConstraint();
        auto searchArea = cb.Settings->SearchArea;
        searchArea->setDimensions(2);
        searchArea->Dimensions[0]->StartValue = 1.0;
        searchArea->Dimensions[1]->StartValue = 1.0;
        auto result = cb.getOptimizedSample(model);
        EXPECT_NEAR(result->optimizedSample->Values[0], 0.707, 1e-2);
        EXPECT_NEAR(result->optimizedSample->Values[1], -0.707, 1e-2);
        EXPECT_NEAR(result->minimumValue, -0.5, 1e-3);
        EXPECT_EQ(result->totalModelRuns, 49);
        EXPECT_TRUE(result->succeeded);
    }

    Models::ZModel TestCobyla::getTestModel(double offset1, double offset2)
    {
        Models::ZLambda function = [offset1, offset2](Models::ModelSample& sample)
        {
            sample.Z = 10.0 * std::pow(sample.Values[0] - offset1, 2) + std::pow(sample.Values[1] - offset2, 2);
        };

        Models::ZModel model = Models::ZModel(function);

        return model;
    }

    Models::ZModel TestCobyla::getTestModelWithConstraint()
    {
        Models::ZLambda function = [](Models::ModelSample& sample)
        {
            sample.Z = sample.Values[0] * sample.Values[1];;
        };

        Models::ZModel model = Models::ZModel(function);

        Models::ZBetaLambda constraint = [](Models::ModelSample& sample)
        {
            double C = 1.0 - hypot(sample.Values[0], sample.Values[1]);
            return std::abs(C);
        };

        model.setConstraint(constraint);

        return model;
    }

}

