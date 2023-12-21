#include "gm_tests.h"
#include "gtest/gtest.h"
#include "../../src/rnd/george_marsaglia.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        // Test getRandomValue for one value with ij and kl both equal to 10000
        void gm_tests::test_first_value()
        {
            const int ij = 10000;
            const int kl = 10000;

            auto gm = george_marsaglia(ij, kl);

            double randomValue = gm.get_uniform();

            EXPECT_NEAR(randomValue, 0.755450129508972, margin);
        }

        void gm_tests::test_last_value()
        {
            const int ij = 177;
            const int kl = 177;
            const int n = 100000;

            auto gm = george_marsaglia(ij, kl);

            double randomValue;
            for (int i = 0; i < n; i++)
            {
                randomValue = gm.get_uniform();
            }

            EXPECT_NEAR(randomValue, 0.7785768508911, margin);
        }
    }
}
