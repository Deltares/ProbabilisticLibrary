#include "pch.h"
#include "testDistributions.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"

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
                    -2.55573190691464,
                    -2.30997013021024,
                    -2.03190840230637,
                    -1.71237406597375,
                    -1.33796203278021,
                    -0.888239603990872,
                    -0.330566071316019,
                    0.389679340615967,
                    1.36651292058166,
                    2.75588794089519,
                    4.77169994533666,
                    7.60705089255662,
                    11.3600856506974,
                    16.0649982505082,
                    21.7367689494396,
                    28.3843074961794
                };

                for (int i = -8; i < 8; i++)
                {
                    auto u = (double)i;
                    auto x = distCondWeibull.getXFromU(u);
                    EXPECT_NEAR(x, expectedValues[i+8], margin);
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
                EXPECT_NEAR(mean, 1.5769074058004957, margin);
                auto deviation = distCondWeibull.getDeviation();
                EXPECT_NEAR(deviation, 1.2808040716532743, margin);
            }

            void testDistributions::testConditionalWeibullCdfPdf()
            {
                std::shared_ptr< StochastProperties> params = std::make_shared< StochastProperties>();
                params->Scale = 1.0;
                params->Shape = 1.0;
                params->Shift = 1.0;
                auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

                std::vector<double> expectedValues = {
                    0, 0,
                    2.50602e-173, 6.21014e-176,
                    0.179374, 0.065988,
                    0.0179832, 0.981851,
                    0.000123395, 0.999877,
                    8.31528e-07, 0.999999,
                    5.60274e-09, 1 };

                int ii = 0;
                for (int i = -2; i < 5; i++)
                {
                    double x = (double)(i*5);
                    double pdf = distCondWeibull.getPDF(x);
                    double cdf = distCondWeibull.getCDF(x);
                    EXPECT_NEAR(pdf, expectedValues[ii++], 1e-4);
                    EXPECT_NEAR(cdf, expectedValues[ii++], 1e-4);
                }
            }
        }
    }
}

