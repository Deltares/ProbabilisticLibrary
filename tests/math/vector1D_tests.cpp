#include <gtest/gtest.h>
#include <cmath>
#include "../../src/utils/vector1D.h"
#include "vector1D_tests.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void vector1D_tests::vector_sum_test()
        {
            auto v1 = vector1D(2);
            auto v2 = vector1D(2);
            v1.assign(2.0);
            v2.assign(3.0);
            auto lv1 = v1.sumOfSquares();
            auto lv2 = v2.sumOfSquares();
            auto inner = v2.sumOfInner(v1);
            EXPECT_DOUBLE_EQ(lv1, 8.0);
            EXPECT_DOUBLE_EQ(lv2, 18.0);
            EXPECT_DOUBLE_EQ(inner, 12.0);
        }

        void vector1D_tests::vector_normalize_test()
        {
            auto v = vector1D({ 0.1, 0.2, 0.3 });
            v.normalize();
            EXPECT_NEAR(v(0), 0.267261, 2e-6);
            EXPECT_NEAR(v(1), 0.534522, 2e-6);
            EXPECT_NEAR(v(2), 0.801784, 2e-6);
            auto s = v.sumOfSquares();
            EXPECT_NEAR(s, 1.0, 1e-10);
        }

        void vector1D_tests::minmax_test()
        {
            auto v = vector1D({ 0.1, 0.2, 0.3 });
            double max = v.maxval();
            double min = v.minval();
            EXPECT_DOUBLE_EQ(v(0), min);
            EXPECT_DOUBLE_EQ(v(2), max);

            auto v2 = vector1D();
            double y = v2.maxval();
            EXPECT_TRUE(std::isnan(y));
        }
    }
}
