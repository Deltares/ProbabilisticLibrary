#include "pch.h"
#include <gtest/gtest.h>
#include "Reliability/testStartPointCalculator.h"
#include "Math/testNumericSupport.h"
#include "Combin/hohenbichler_tests.h"
#include "Combin/intEqualElements_tests.h"
#include "Combin/upscale_tests.h"
#include "Combin/combinElements_tests.h"
#include "Combin/hohenbichlerCombiner_tests.h"
#include "Distributions/testDistributions.h"

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

            TEST(unittst, testHohenbichlerCombinerElements)
            {
                auto h = HohenbichlerCombinerTest();
                h.allHohenbichlerCombinerTests();
            }

            TEST(unittst, testCombinElements)
            {
                auto h = combinElementsTests();
                h.runAll();
            }

            TEST(unittst, testUpscaling)
            {
                auto h = upscale_tests();
                h.runAll();
            }

            TEST(unittst, testIntEqualElements)
            {
                auto h = intEqualElements_tests();
                h.allIntegrateEqualElementsTests();
            }

            TEST(unittst, testHohenbichler)
            {
                auto h = HohenbichlerTest();
                h.allHohenbichlerTests();
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

            TEST(unittst, testDistributions)
            {
                auto tstDist = testDistributions();
                tstDist.allDistributionsTests();
            }
        }
    }
}

