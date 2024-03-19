#include <gtest/gtest.h>
#include "Reliability/testStartPointCalculator.h"
#include "Math/testNumericSupport.h"

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
                return RUN_ALL_TESTS();
            }

            TEST(unittst, testNumericSupport)
            {
                auto tstNumSupport = testNumericSupport();
                tstNumSupport.allNumericSupportTests();
            }

            TEST(unittst, testStartPointCalculator)
            {
                auto tstStartPoint = testStartPointCalculator();
                tstStartPoint.allStartPointTests();
            }

        }
    }
}

