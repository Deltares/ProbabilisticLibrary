#include <cmath>
#include "basic_math_tests.h"
#include "gtest/gtest.h"
#include "../../src/utils/basic_math.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {
        void basic_math_tests::test_min()
        {
            int i = 23;
            int j = 45;
            int t1 = min(i, j);
            EXPECT_EQ(t1, i);

            float x = 23.4f;
            float y = 45.6f;
            float t2 = min(x, y);
            EXPECT_EQ(t2, x);

            double d = 34.5;
            double e = 67.8;
            double t3 = min(d, e);
            EXPECT_EQ(t3, d);
        }

        void basic_math_tests::test_max()
        {
            int i = 23;
            int j = 45;
            int t1 = max(i, j);
            EXPECT_EQ(t1, j);

            float x = 23.4f;
            float y = 45.6f;
            float t2 = max(x, y);
            EXPECT_EQ(t2, y);

            double d = 34.5;
            double e = 67.8;
            double t3 = max(d, e);
            EXPECT_EQ(t3, e);
        }

        void basic_math_tests::test_interp()
        {
            // x is halfway x1 and x2, so the answer is halfway y1 and y2
            // test for doubles
            auto t1d = interpLine(2.0, 1.0, 3.0, 4.0, 6.0);
            EXPECT_NEAR(t1d, 5.0, margin);

            // test for floats
            auto t1f = interpLine(2.0f, 1.0f, 3.0f, 4.0f, 6.0f);
            EXPECT_NEAR(t1f, 5.0f, marginf);

            // y1 and y2 are the same, so the answer is y1 (and y2)
            // test for doubles
            auto t2d = interpLine(1.5, 1.0, 2.0, 3.0, 3.0);
            EXPECT_NEAR(t2d, 3.0, margin);

            // test for floats
            auto t2f = interpLine(1.5f, 1.0f, 2.0f, 3.0f, 3.0f);
            EXPECT_NEAR(t2f, 3.0f, marginf);

            // check linear extrapolation; as x is as far from x1 as x2 from x2, the answer is y1 - (y2-y1) = 2.0
            // test for doubles
            auto t3d = interpLine(0.0, 1.0, 2.0, 4.0, 6.0);
            EXPECT_NEAR(t3d, 2.0, margin);

            // test for floats
            auto t3f = interpLine(0.0f, 1.0f, 2.0f, 4.0f, 6.0f);
            EXPECT_NEAR(t3f, 2.0f, marginf);

            // check division by zero
            // for doubles
            auto t4d = interpLine(2.0, 3.0, 3.0, 5.0, 6.0);
            auto nancheck = std::isnan(t4d);
            EXPECT_TRUE(nancheck);

            // for floats
            auto t4f = interpLine(2.0f, 3.0f, 3.0f, 5.0f, 6.0f);
            auto nancheckf = std::isnan(t4f);
            EXPECT_TRUE(nancheckf);
        }

        void basic_math_tests::test_sign()
        {
            EXPECT_EQ(1, sign(1, 2));
            EXPECT_EQ(-3, sign(3, -4));
            EXPECT_EQ(5, sign(-5, 6));
            EXPECT_EQ(-7, sign(-7, -8));
            EXPECT_EQ(3.14, sign(-3.14, 2.78));
            EXPECT_EQ(-3.14f, sign(3.14f, -2.78f));
        }
    }
}
