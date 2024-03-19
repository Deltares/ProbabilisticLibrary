#include "testNumericSupport.h"
#include "../../Deltares.Probabilistic/Math/NumericSupport.h"

using namespace Deltares::Numeric;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void testNumericSupport::allNumericSupportTests()
            {
                testLinearInterpolate();
                testLogLinearInterpolate();
                testHarmonicInterpolate();
            }

            void testNumericSupport::testLinearInterpolate()
            {
                auto x = std::vector<double>{ 0.0, 1.0 };
                auto y = std::vector<double>{ 3.0, 4.0 };
                auto result = NumericSupport::interpolate(0.5, x, y);
                EXPECT_NEAR(result, 3.5, margin);

                result = NumericSupport::interpolate(2.0, x, y, true);
                EXPECT_NEAR(result, 5.0, margin);

                result = NumericSupport::interpolate(2.0, x, y);
                EXPECT_NEAR(result, 4.0, margin);
            }

            void testNumericSupport::testLogLinearInterpolate()
            {
                auto x = std::vector<double>{ 1.0, 4.0 };
                auto y = std::vector<double>{ 3.0, 4.0 };
                auto result = NumericSupport::interpolate(2.0, x, y, false, Logarithmic);
                EXPECT_NEAR(result, 3.5, margin);

                result = NumericSupport::interpolate(8.0, x, y, true, Logarithmic);
                EXPECT_NEAR(result, 4.5, margin);

                result = NumericSupport::interpolate(8.0, x, y, false, Logarithmic);
                EXPECT_NEAR(result, 4.0, margin);
            }

            void testNumericSupport::testHarmonicInterpolate()
            {
                auto x = std::vector<double>{ 1.0, 4.0 };
                auto y = std::vector<double>{ 3.0, 4.0 };
                auto result = NumericSupport::interpolate(2.0, x, y, false, Harmonic);
                EXPECT_NEAR(result, 3.0 + 2.0/3.0, margin);

                result = NumericSupport::interpolate(8.0, x, y, true, Harmonic);
                EXPECT_NEAR(result, 4.0 + 1.0/6.0, margin);

                result = NumericSupport::interpolate(8.0, x, y, false, Harmonic);
                EXPECT_NEAR(result, 4.0, margin);
            }

        }
    }
}

