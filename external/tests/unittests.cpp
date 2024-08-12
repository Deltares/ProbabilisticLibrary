#include <gtest/gtest.h>
#include "../src/distributions/distrib.h"
#include "rnd/gm_tests.h"
#include "rnd/normVectorTests.h"
#include "correlation/gaussian_correlation_tests.h"
#include "combin/hohenbichler_tests.h"
#include "distrib/distributionsTests.h"
#include "combin/combinElements_tests.h"
#include "combin/upscale_tests.h"
#include "combin/intEqualElements_tests.h"
#include "probFuncs/probNumber_tests.h"
#include "rootFinder/root_finder_tests.h"
#include "rootFinder/sphere_tasks_tests.h"
#include "strings/plStringTests.h"
#include "probFuncs/conversion_tests.h"
#include "../src/extern/dgammaDs.h"
#include "correlation/testTrapeziumRule.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        int main(int argc, char** argv)
        {
            ::testing::InitGoogleTest(&argc, argv);
            //::testing::GTEST_FLAG(filter) = "unittst.trapeziumRuleTests";
            return RUN_ALL_TESTS();
        }

        TEST(unittst, plstringtests)
        {
            auto plst = plStringTests();
            plst.allPlStringTests();
        }

        TEST(unittst, spheretaskstests)
        {
            auto sftt = sphere_tasks_tests();
            sftt.all_sphere_tasks_tests();
        }

        TEST(unittst, rootfindertests)
        {
            auto rf = root_finder_tests();
            rf.all_root_finder_tests();
        }

        TEST(unittst, probNumbertests)
        {
            auto pnTest = probNumber_tests();
            pnTest.all_probNumber_tests();
        }

        TEST(unittst, upscalingTests)
        {
            auto up = upscale_tests();
            up.runAll();
        }

        TEST(unittst, intEqualElementTests)
        {
            auto intEE = intEqualElements_tests();
            intEE.integrateEqualElementsTests1();
            intEE.integrateEqualElementsTests2();
            intEE.integrateEqualElementsTests3();
            intEE.integrateEqualElementsTests4();
        }

        TEST(unittst, combineElements)
        {
            auto cmb = combinElementsTests();
            cmb.runAll();
        }

        TEST(unittst, distributions_tests)
        {
            auto db = distributionsTests();
            db.allTests();
        }

        TEST(unittst, hohenbichler_tests)
        {
            auto h = HohenbichlerTest();
            h.NoCorrelation();
            h.AlmostFullCorrelation();
            h.FullCorrelation();
            h.PartialCorrelation();
            h.negativeCorrelation();
            h.negativeCorrelationNegativeBeta();
            h.negativeCorrelationZeroBeta();
        }

        TEST(unittst, george_marsaglia_test)
        {
            auto t = gm_tests();
            t.test_first_value();
            t.test_last_value();
        }

        TEST(unittst, gaussian_correlation_tests)
        {
            auto t = gaussian_correlation_tests();
            t.testCorrelationGaussianModel1a();
            t.testCorrelationGaussianModel2a();
            t.testCorrelationGaussianModel1b();
            t.testCorrelationGaussianModel2b();
        }

        TEST(unittst, conversionTests)
        {
            auto convTests = conversion_tests();
            convTests.all_conversion_tests();
        }

        TEST(unittst, dgammaqTests)
        {
            auto ds = dgammaDs(10.5);
            for (int i = 100; i < 170; i++)
            {
                double r = 0.1 * (double)i;
                double y = ds.Dgammq(10.5, 0.5 * r * r);
                double y2 = 1 - conversions::Dgammq(0.5 * r * r, 10.5);
                if (y > 1e-14 && y2 > 1e-14)
                {
                    EXPECT_NEAR(y, y2, 1e-16);
                }
            }

        }

        TEST(unittst, trapeziumRuleTests)
        {
            auto trapeziumTests = testTrapeziumRule();
            trapeziumTests.allTrapeziumRuleTests();
        }
    }
}
