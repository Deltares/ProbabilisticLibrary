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
                    auto x = distCondWeibull.getXFromU(i);
                    EXPECT_NEAR(x, expectedValues[i+8], margin);
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
        }
    }
}

