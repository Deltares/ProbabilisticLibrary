// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include <gtest/gtest.h>
#include "testCobyla.h"

using namespace Deltares::Optimization;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void testCobyla::allCobylaTests()
            {
                test_with_constraint1();
                test_no_constraints1();
                test_no_constraints2();
            }

            void testCobyla::test_no_constraints1()
            {
                auto cb = CobylaOptimization();
                auto model = std::make_shared<testModel>();
                auto searchArea = SearchArea();
                searchArea.Dimensions = std::vector<SearchDimension>(2);
                auto result = cb.GetCalibrationPoint(searchArea, model);
                EXPECT_NEAR(result.Input[0], -1.0, 1e-3);
                EXPECT_NEAR(result.Input[1], 0.0, 1e-3);
            }

            void testCobyla::test_no_constraints2()
            {
                auto cb = CobylaOptimization();
                auto model = std::make_shared<testModel>(2, 3);
                auto searchArea = SearchArea();
                searchArea.Dimensions = std::vector<SearchDimension>(2);
                auto result = cb.GetCalibrationPoint(searchArea, model);
                EXPECT_NEAR(result.Input[0], 2.0, 1e-3);
                EXPECT_NEAR(result.Input[1], 3.0, 1e-3);
            }

            void testCobyla::test_with_constraint1()
            {
                auto cb = CobylaOptimization();
                auto model = std::make_shared<testModelWithConstraint>();
                auto searchArea = SearchArea();
                searchArea.Dimensions = std::vector<SearchDimension>(2);
                searchArea.Dimensions[0].StartValue = 1.0;
                searchArea.Dimensions[1].StartValue = 1.0;
                auto result = cb.GetCalibrationPoint(searchArea, model);
                EXPECT_NEAR(result.Input[0], 0.707, 1e-2);
                EXPECT_NEAR(result.Input[1], -0.707, 1e-2);
            }

            double testModel::GetZValue(std::shared_ptr<Models::Sample> sample) const
            {
                return 10.0 * std::pow(sample->Values[0] - offset1, 2) + std::pow(sample->Values[1] - offset2, 2);
            };

            double testModelWithConstraint::GetZValue(std::shared_ptr<Models::Sample> sample) const
            {
                double Z = sample->Values[0] * sample->Values[1];
                return Z;
            }

            double testModelWithConstraint::GetConstraintValue(const Models::Sample& sample) const
            {   // constraint: point lies on unit sphere
                double C = 1.0 - hypot(sample.Values[0], sample.Values[1]);
                return std::abs(C);
            }

        }
    }
}

