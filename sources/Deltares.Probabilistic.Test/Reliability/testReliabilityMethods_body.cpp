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
#include "testReliabilityMethods.h"
#include "../../Deltares.Probabilistic/Reliability/LatinHyperCube.h"
#include "../../Deltares.Probabilistic/Reliability/NumericalBisection.h"
#include "../../Deltares.Probabilistic/Reliability/CobylaReliability.h"
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
            void testReliabilityMethods::allReliabilityMethods() const
            {
                testCobylaReliability();
                testLatinHyperCube();
                testNumericalBisection();
                testNumericalBisectionLinear();
            }

            void testReliabilityMethods::testLatinHyperCube() const
            {
                auto calculator = LatinHyperCube();
                calculator.Settings->randomSettings->RandomGeneratorType = Numeric::MersenneTwister;

                auto modelRunner = projectBuilder().BuildProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                EXPECT_NEAR(designPoint->Beta, 1.8250068211, margin);
                EXPECT_NEAR(designPoint->Alphas[0]->Alpha, -0.5644677927, margin);
                EXPECT_NEAR(designPoint->Alphas[1]->Alpha, 0.8254550932, margin);
            }

            void testReliabilityMethods::testNumericalBisection() const
            {
                auto calculator = NumericalBisection();
                calculator.Settings->MaximumIterations = 20;

                auto modelRunner = projectBuilder().BuildProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                ASSERT_NEAR(designPoint->Beta, 1.87406654375, margin);
            }

            void testReliabilityMethods::testNumericalBisectionLinear() const
            {
                auto calculator = std::make_shared<NumericalBisection>();

                auto project = projectBuilder::getLinearProject();
                project->reliabilityMethod = calculator;
                project->settings->ReliabilityMethod = ReliabilityNumericalBisection;
                project->settings->MaximumIterations = 20;

                auto designPoint = project->getDesignPoint();

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                ASSERT_NEAR(designPoint->Beta, 2.57, 0.01);
            }

            void testReliabilityMethods::testCobylaReliability() const
            {
                auto calculator = CobylaReliability();

                auto modelRunner = projectBuilder().BuildProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                EXPECT_NEAR(designPoint->Beta, 1.8250068211, margin);
                EXPECT_NEAR(designPoint->Alphas[0]->Alpha, -0.5644677927, margin);
                EXPECT_NEAR(designPoint->Alphas[1]->Alpha, 0.8254550932, margin);
            }


        }

    }
}
