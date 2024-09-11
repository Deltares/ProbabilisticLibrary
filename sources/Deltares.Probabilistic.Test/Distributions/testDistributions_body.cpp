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
#include "testDistributions.h"

using namespace Deltares::Statistics;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void testDistributions::allDistributionsTests()
            {
                testConditionalWeibull();
                testConditionalWeibullNonIntegerShape();
                testConditionalWeibullMeanDeviation();
                testConditionalWeibullCdfPdf();
                testConditionalWeibullCdfPdf2();
            }

            void testDistributions::testConditionalWeibull()
            {
                std::shared_ptr< StochastProperties> params = std::make_shared< StochastProperties>();
                params->Scale = 1.0;
                params->Shape = 1.0;
                params->Shift = 1.0;
                auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

                std::vector<double> expectedValues = 
                {
                    1.36651292058166,
                    2.75588794089519,
                    4.77169994533666,
                    7.60705089255662,
                    11.3600856506974,
                    16.0649982505082,
                    21.7367689494396,
                    28.3843074961794
                };

                for (int i = 0; i < expectedValues.size(); i++)
                {
                    auto u = (double)i;
                    auto x = distCondWeibull.getXFromU(u);
                    EXPECT_NEAR(x, expectedValues[i], margin);
                    auto uCalculated = distCondWeibull.getUFromX(x);
                    EXPECT_NEAR(u, uCalculated, 5.0*margin);
                }

            }

            void testDistributions::testConditionalWeibullNonIntegerShape()
            {
                std::shared_ptr< StochastProperties> params = std::make_shared< StochastProperties>();
                params->Scale = 1.0;
                params->Shape = 1.001;
                params->Shift = 1.0;
                auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

                for (int i = -8; i < -2; i++)
                {
                    auto x = distCondWeibull.getXFromU(i);
                    EXPECT_NEAR(x, 0.0, margin);
                }
            }

            void testDistributions::testConditionalWeibullMeanDeviation()
            {
                std::shared_ptr< StochastProperties> params = std::make_shared< StochastProperties>();
                params->Scale = 1.0;
                params->Shape = 1.0;
                params->Shift = 1.0;
                auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

                auto mean = distCondWeibull.getMean();
                EXPECT_NEAR(mean, 1.5956017719, margin);
                auto deviation = distCondWeibull.getDeviation();
                EXPECT_NEAR(deviation, 1.2539175933, margin);
            }

            double testDistributions::getPdfNumerical(Stochast & s, const double x)
            {
                const double dx = 1e-4;
                double p2 = s.getCDF(x + dx);
                double p1 = s.getCDF(x - dx);
                double pdf = (p2 - p1) / (2.0 * dx);
                return pdf;
            }

            void testDistributions::testConditionalWeibullCdfPdf()
            {
                auto params = std::make_shared<StochastProperties>();
                params->Scale = 1.0;
                params->Shape = 1.0;
                params->Shift = 1.0;
                auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

                std::vector<double> expectedValues = {
                    0.981851,
                    0.999877,
                    0.999999,
                    1 };

                for (int i = 1; i < 5; i++)
                {
                    double x = (double)(i * 5);
                    double cdf = distCondWeibull.getCDF(x);
                    EXPECT_NEAR(cdf, expectedValues[i-1], 1e-4);
                }

                for (int i = 1; i < 10; i++)
                {
                    double x = (double)i;
                    double pdf = distCondWeibull.getPDF(x);
                    double expectedPdf = getPdfNumerical(distCondWeibull, x);
                    EXPECT_NEAR(pdf, expectedPdf, 1e-4);
                }
            }

            void testDistributions::testConditionalWeibullCdfPdf2()
            {
                auto params = std::make_shared<StochastProperties>();
                params->Scale = 2.0;
                params->Shape = 2.0;
                params->Shift = 2.0;
                auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

                for (int i = 1; i < 10; i++)
                {
                    double x = (double)i;
                    double pdf = distCondWeibull.getPDF(x);
                    double expectedPdf = getPdfNumerical(distCondWeibull, x);
                    EXPECT_NEAR(pdf, expectedPdf, 1e-4);
                }
            }

        }
    }
}

