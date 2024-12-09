// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
                testFreqFromBeta();
                testLogQFromBeta();
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
                    double rt = StandardNormal::getTFromU(beta);
                    double beta2 = StandardNormal::getUFromT(rt);
                    EXPECT_NEAR(beta, beta2, margin);
                }

                // known relation 10,000 years gives beta = 3.719...
                double rt = StandardNormal::getUFromT(10000.0);
                EXPECT_NEAR(rt, 3.71902911644, margin);
            }

            // Test of method getFreqFromU
            // Comparison with results per-computed with Matlab
            void testStandardNormal::testFreqFromBeta()
            {
                // expected frequency, pre - computed with Matlab
                auto freq_expected = std::vector<double>({ 0.693147180559945, 0.172753779023450, 0.0230129093289635,
                    0.00135080996474820, 3.16717433774893e-5, 2.86651613008105e-7, 9.86587645037701e-10, 1.27981254388584e-12 });

                for (size_t i = 0; i < freq_expected.size(); i++)
                {
                    auto beta = (float)i;
                    auto freq = StandardNormal::getFreqFromU(beta);
                    EXPECT_NEAR(freq, freq_expected[i], margin);
                }

            }

            // Test of function LogQFromBeta
            // Comparison with results per-computed with Matlab
            void testStandardNormal::testLogQFromBeta()
            {
                // expected -log(Q), pre-computed with Matlab
                auto logQ_expected = std::vector<double>({0.366512920581664, 1.75588794089519, 3.77169994533666,
                    6.60705089255662, 10.3600856506974, 15.0649982505082, 20.7367689499747, 27.3843074962 });

                for (size_t i = 0; i < logQ_expected.size(); i++)
                {
                    auto beta = (float)i;
                    auto logQ = StandardNormal::getLogQFromU(beta);
                    EXPECT_NEAR(logQ, logQ_expected[i], margin);
                }
            }

        }
    }
}

