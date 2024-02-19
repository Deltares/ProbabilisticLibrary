#include <iostream>
#include <gtest/gtest.h>
#include "combin/hohenbichler_tests.h"

namespace Deltares
{
    namespace ProbLibTests
    {
        int main(int argc, char** argv)
        {
            ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();
        }

        TEST(unittst, hohenbichler_tests)
        {
            auto h = HohenbichlerTest();
            h.RhoLimit();
        }
    }
}
