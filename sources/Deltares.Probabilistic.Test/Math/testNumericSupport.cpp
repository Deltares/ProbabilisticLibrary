#ifndef NOPCH
#include "pch.h"
#endif // !NOPCH
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
                testMinMaxFunctions();
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

            void testNumericSupport::testMinMaxFunctions()
            {
                auto numbers = std::vector<double> { -2.0, 2.0, -3.0, 3.0, 1.0, -1.0, 0.0 };

                auto maxval = NumericSupport::getMaximum(numbers);
                EXPECT_EQ(maxval, 3.0);

                auto minval = NumericSupport::getMinimum(numbers);
                EXPECT_EQ(minval, -3.0);

                auto locmin = NumericSupport::getLocationMinimum(numbers);
                EXPECT_EQ(locmin, 2);

                auto locmax = NumericSupport::getLocationMaximum(numbers);
                EXPECT_EQ(locmax, 3);

                for (auto& x : numbers) { x = std::abs(x); }
                minval = NumericSupport::getMinimum(numbers);
                EXPECT_EQ(minval, 0.0);

                locmax = NumericSupport::getLocationMinimum(numbers);
                EXPECT_EQ(locmax, 6);

            }
        }
    }
}

