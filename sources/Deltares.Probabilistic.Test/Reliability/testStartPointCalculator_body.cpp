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
#include <cmath>
#include "testStartPointCalculator.h"
#include "../../Deltares.Probabilistic/Reliability/StartPointCalculator.h"
#include "../projectBuilder.h"

using namespace Deltares::Reliability;
using namespace Deltares::Statistics;
using namespace Deltares::Models;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void testStartPointCalculator::allStartPointTests()
            {
                testMethodOne();
                testMethodRaySearch();
                testMethodSphereSearch();
                testMethodSphereSearchAllDirections();
                testMethodSphereSearchWithDeterminist();
            }

            void testStartPointCalculator::testMethodOne()
            {
                auto modelRunner = projectBuilder().BuildProject();
                auto calculator = StartPointCalculator();

                modelRunner->updateStochastSettings(calculator.Settings->StochastSet);
                calculator.Settings->StartMethod = StartMethodType::FixedValue;
                calculator.Settings->startVector = { 1.0, 1.0 };

                auto r = calculator.getStartPoint(modelRunner);

                ASSERT_EQ(r->Values.size(), 2);
                EXPECT_EQ(r->Values[0], 1.0);
                EXPECT_EQ(r->Values[1], 1.0);
            }

            void testStartPointCalculator::testMethodRaySearch()
            {
                auto modelRunner = projectBuilder().BuildProject();
                auto calculator = StartPointCalculator();

                modelRunner->updateStochastSettings(calculator.Settings->StochastSet);
                calculator.Settings->StartMethod = StartMethodType::RaySearch;
                calculator.Settings->MaximumLengthStartPoint = 20.0;
                calculator.Settings->dsdu = 3.0;

                auto r = calculator.getStartPoint(modelRunner);

                ASSERT_EQ(r->Values.size(), 2);
                EXPECT_NEAR(r->Values[0], 12.0, margin);
                EXPECT_NEAR(r->Values[1], 12.0, margin);
            }

            void testStartPointCalculator::testMethodSphereSearch()
            {
                auto modelRunner = projectBuilder().BuildProject();
                auto calculator = StartPointCalculator();

                modelRunner->updateStochastSettings(calculator.Settings->StochastSet);
                calculator.Settings->StartMethod = StartMethodType::SphereSearch;

                auto r = calculator.getStartPoint(modelRunner);

                ASSERT_EQ(r->Values.size(), 2);
                auto z = modelRunner->getZValue(r);
                EXPECT_TRUE(std::abs(z) < margin);
                EXPECT_NEAR(r->Values[0], 2.4, margin);
                EXPECT_NEAR(r->Values[1], 0.0, margin);
            }

            void testStartPointCalculator::testMethodSphereSearchAllDirections()
            {
                auto modelRunner = projectBuilder().BuildProject();
                auto calculator = StartPointCalculator();

                modelRunner->updateStochastSettings(calculator.Settings->StochastSet);
                calculator.Settings->StartMethod = StartMethodType::SphereSearch;
                calculator.Settings->allQuadrants = true;
                calculator.Settings->maxStepsSphereSearch = 16;

                auto r = calculator.getStartPoint(modelRunner);

                ASSERT_EQ(r->Values.size(), 2);
                auto z = modelRunner->getZValue(r);
                EXPECT_TRUE(std::abs(z) < margin);
                EXPECT_NEAR(r->Values[0], 1.64172137689, margin);
                EXPECT_NEAR(r->Values[1], -0.94784827888, margin);
            }

            void testStartPointCalculator::testMethodSphereSearchWithDeterminist()
            {
                auto modelRunner = projectBuilder().BuildProjectWithDeterminist(3.0);
                auto calculator = StartPointCalculator();

                modelRunner->updateStochastSettings(calculator.Settings->StochastSet);
                calculator.Settings->StartMethod = StartMethodType::SphereSearch;

                auto r = calculator.getStartPoint(modelRunner);

                ASSERT_EQ(r->Values.size(), 2);
                EXPECT_NEAR(r->Values[1], 2.4, margin);
                EXPECT_NEAR(r->Values[0], 0.0, margin);
            }

        }
    }
}
