#include <gtest/gtest.h>
#include "Reliability/testStartPointCalculator.h"

using namespace Deltares::Probabilistic;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            int main(int argc, char** argv)
            {
                ::testing::InitGoogleTest(&argc, argv);
                //::testing::GTEST_FLAG(filter) = "unittst.testStartPointCalculator";
                return RUN_ALL_TESTS();
            }

            TEST(unittst, testStartPointCalculator)
            {
                auto tstStartPoint = testStartPointCalculator();
                tstStartPoint.allStartPointTests();
            }

        }
    }
}

