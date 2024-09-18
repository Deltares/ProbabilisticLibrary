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
#include "testStandardNormal.h"
#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"

using namespace Deltares::Statistics;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void testStandardNormal::allStandardNormalTests()
            {
                testPQbetaConversions();
                testReturnTime();
            }

            void testStandardNormal::testPQbetaConversions()
            {
                double beta = 0.0;
                auto pq = StandardNormal::getPQFromU(beta);
                ASSERT_EQ(pq.p, pq.q); // both 0.5

                auto a = StandardNormal::getUFromP(0.0);
                auto b = StandardNormal::getUFromQ(0.0);
                ASSERT_EQ(a, -StandardNormal::BetaMax);
                ASSERT_EQ(b, StandardNormal::BetaMax);
            }

            void testStandardNormal::testReturnTime()
            {
                for (int i = -70; i < 70; i++)
                {
                    double beta = 0.1 * (double)i;
                    double rt = StandardNormal::ReturnTimeFromBeta(beta);
                    double beta2 = StandardNormal::BetaFromReturnTime(rt);
                    EXPECT_NEAR(beta, beta2, margin);
                }

                // known relation 10,000 years gives beta = 3.719...
                double rt = StandardNormal::BetaFromReturnTime(10000.0);
                EXPECT_NEAR(rt, 3.71902911644, margin);
            }

        }
    }
}

