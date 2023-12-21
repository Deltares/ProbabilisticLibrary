#include <cmath>
#include "gtest/gtest.h"
#include "root_finder_tests.h"
#include "../../src/rootFinders/linear_loop.h"
#include "../../src/rootFinders/bisection.h"
#include "../../src/rootFinders/regula_falsi.h"
#include "../../src/rootFinders/Anderson_Bjork.h"
#include "../../src/probMethods/dsRootFinderRobust.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void root_finder_tests::all_root_finder_tests()
        {
            anderson_bjork_outside_x0_x1_test();
            anderson_bjork_nod_test();
            linear_loop_test_par();
            linear_loop_test5();
            linear_loop_test4();
            linear_loop_test3();
            linear_loop_test2();
            linear_loop_test1();
            bisection_test1();
            regula_falsi_test1();
            regula_falsi_test2();
            regula_falsi_test3();
            regula_falsi_test4();
            anderson_bjork_test1();
            dsRootFinder_test1();
            dsRootFinder_test2a();
            dsRootFinder_test2b();
            dsRootFinder_test3();
            dsRootFinder_testNod();
            dsRootFinder_testNodRobust();
            dsRootFinder_testNodRobustBisection();
        }

        void root_finder_tests::linear_loop_test1()
        {
            auto func = sampleFunc1();
            auto rf = linear_loop(func);
            auto result = rf.FindRoot(0.0, 7.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, 3.0, 0.2);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 19);
        }

        void root_finder_tests::linear_loop_test2()
        {
            auto func = sampleFunc1();
            auto rf = linear_loop(func, 1e-3, 10, ll_final_step::lastx, 1);
            auto result = rf.FindRoot(4.0, 7.0);
            ASSERT_FALSE(result.first);
            ASSERT_NEAR(result.second, 7.0, 1e-9);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 11);
        }

        void root_finder_tests::linear_loop_test3()
        {
            auto func = sampleFunc1();
            auto rf = linear_loop(func, 1e-3, 5, ll_final_step::interp, 1);
            auto result = rf.FindRoot(0.0, 7.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, 3.0, 1e-9); // method is exact for a linear function even with big steps
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 4);
        }

        void root_finder_tests::linear_loop_test4()
        {
            auto func = sampleFunc2();
            auto rf = linear_loop(func, 1e-3, 5, ll_final_step::interp, 1);
            auto result = rf.FindRoot(1.0, 2.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, sqrt(3.0), 0.1);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 5);
        }

        void root_finder_tests::linear_loop_test5()
        {
            auto func = sampleFunc2();
            auto rf = linear_loop(func, 1e-3, 5, ll_final_step::xminz, 1);
            auto result = rf.FindRoot(1.0, 2.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, sqrt(3.0), 0.1);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 5);
        }

        void root_finder_tests::linear_loop_test_par()
        {
            auto func = sampleFunc2();
            auto rf = linear_loop(func, 1e-3, 5, ll_final_step::xminz, 4);
            auto result = rf.FindRoot(1.0, 2.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, sqrt(3.0), 0.1);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 5);
        }

        void root_finder_tests::bisection_test1()
        {
            auto func = sampleFunc1();
            auto rf = bisection(func);
            auto result = rf.FindRoot(0.0, 7.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, 3.0, 1e-2);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 9);
        }

        void root_finder_tests::regula_falsi_test1()
        {
            auto func = sampleFunc1();
            auto rf = regula_falsi(func);
            auto result = rf.FindRoot(0.0, 7.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, 3.0, 1e-2);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 3);
        }

        void root_finder_tests::regula_falsi_test2()
        {
            auto func = sampleFunc2();
            auto rf = regula_falsi(func);
            auto result = rf.FindRoot(0.0, 7.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, sqrt(3.0), 1e-2);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 17);
        }

        void root_finder_tests::regula_falsi_test3()
        {
            auto func = sampleFunc2();
            auto rf = regula_falsi(func, 1e-5, 100);
            auto result = rf.FindRoot(1.0, 2.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, sqrt(3.0), 1e-5);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 7);
        }

        void root_finder_tests::regula_falsi_test4()
        {
            auto func = sampleFunc2();
            auto rf = regula_falsi(func, 1e-5, 100);
            auto result = rf.FindRoot(-2.0, -1.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, -sqrt(3.0), 1e-5);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 7);
        }

        void root_finder_tests::anderson_bjork_test1()
        {
            auto func = sampleFunc2();
            auto rf = Anderson_Bjork(func, 1e-5, 100);
            auto result = rf.FindRoot(-2.0, -1.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, -sqrt(3.0), 1e-5);
            double y; size_t ii;
            rf.getStats(y, ii);
            ASSERT_EQ(ii, 6);
        }

        void root_finder_tests::anderson_bjork_nod_test()
        {
            auto func = nodFunc();
            auto ab = Anderson_Bjork(func, 5e-3, 10);
            auto result = ab.FindRoot(-10.0, 10.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, 4.0, 5e-3);
            double y; size_t ii;
            ab.getStats(y, ii);
            ASSERT_EQ(ii, 7);
        }

        void root_finder_tests::anderson_bjork_outside_x0_x1_test()
        {
            auto func = nodFunc();
            auto ab = Anderson_Bjork(func, 5e-3, 10);
            auto result = ab.FindRoot(6.0, 8.0);
            ASSERT_TRUE(result.first);
            ASSERT_NEAR(result.second, 4.0, 5e-3);
            double y; size_t ii;
            ab.getStats(y, ii);
            ASSERT_EQ(ii, 3);

            auto result2 = ab.FindRoot(-3.0, 3.0);
            ASSERT_TRUE(result2.first);
            ASSERT_NEAR(result2.second, 4.0, 5e-3);
            ab.getStats(y, ii);
            ASSERT_EQ(ii, 5);

            auto func2 = sampleFunc2();
            auto ab2 = Anderson_Bjork(func2, 1e-5, 25);
            auto result3 = ab2.FindRoot(2.0, 3.0);
            ASSERT_TRUE(result3.first);
            ASSERT_NEAR(result3.second, sqrt(3.0), 1e-5);
            ab2.getStats(y, ii);
            ASSERT_EQ(ii, 5);

            auto result4 = ab2.FindRoot(-3.0, -2.0);
            ASSERT_TRUE(result4.first);
            ASSERT_NEAR(result4.second, -sqrt(3.0), 1e-5);
            ab2.getStats(y, ii);
            ASSERT_EQ(ii, 6);
        }

        void root_finder_tests::dsRootFinder_test1()
        {
            auto func = sampleFunc1();
            auto rtSettings = DSrootFinderSettings();
            double z0 = func.GetValue(0.0);
            auto rootFinder = dsRootFinder(rtSettings);
            rootFinder.init(z0);
            auto result = rootFinder.FindRoot(func, 0.0, maximumLengthU);
            double y; size_t ii;
            func.getStats(y, ii);
            EXPECT_TRUE(result.first);
            EXPECT_NEAR(result.second, 3.0, rtSettings.epsilonDu);
            EXPECT_EQ(ii, 2);
            EXPECT_NEAR(y, 0.0, 1e-3);
        };

        void root_finder_tests::dsRootFinder_test2a()
        {
            auto func = sampleFunc2();
            auto rtSettings = DSrootFinderSettings();
            double z0 = func.GetValue(0.0);
            auto rootFinder = dsRootFinder(rtSettings);
            rootFinder.init(z0);
            auto result = rootFinder.FindRoot(func, 0.0, maximumLengthU);
            double y; size_t ii;
            func.getStats(y, ii);
            EXPECT_TRUE(result.first);
            EXPECT_NEAR(result.second, sqrt(3.0), rtSettings.epsilonDu);
            EXPECT_EQ(ii, 8);
            EXPECT_NEAR(y, 0.0, 1e-2);
        }

        void root_finder_tests::dsRootFinder_test2b()
        {
            auto func = sampleFunc2();
            auto rtSettings = DSrootFinderSettings();
            rtSettings.iterationMethod = DSiterationMethods::DirSamplingIterMethodFastBisection;
            double z0 = func.GetValue(0.0);
            auto rootFinder = dsRootFinder(rtSettings);
            rootFinder.init(z0);
            auto result = rootFinder.FindRoot(func, 0.0, maximumLengthU);
            double y; size_t ii;
            func.getStats(y, ii);
            EXPECT_TRUE(result.first);
            EXPECT_NEAR(result.second, sqrt(3.0), rtSettings.epsilonDu);
            EXPECT_EQ(ii, 14);
            EXPECT_NEAR(y, 0.0, 1e-3);
        };

        void root_finder_tests::dsRootFinder_testNod()
        {
            auto func = nodFunc();
            auto rtSettings = DSrootFinderSettings();
            double z0 = func.GetValue(0.0);
            auto rootFinder = dsRootFinder(rtSettings);
            rootFinder.init(z0);
            auto result = rootFinder.FindRoot(func, 0.0, maximumLengthU);
            double y; size_t ii;
            func.getStats(y, ii);
            EXPECT_TRUE(result.first);
            EXPECT_NEAR(result.second, 4.0, rtSettings.epsilonDu);
            EXPECT_EQ(ii, 3);
            EXPECT_NEAR(y, 0.0, 1e-3);
        };

        void root_finder_tests::dsRootFinder_testNodRobust()
        {
            auto func = nodFunc();
            auto rtSettings = DSrootFinderSettings();
            rtSettings.iterationMethod = DSiterationMethods::DirSamplingIterMethodRobust;
            double z0 = func.GetValue(0.0);
            auto rootFinder = dsRootFinderRobust(rtSettings);
            rootFinder.init(z0);
            auto result = rootFinder.FindRoot(func, 0.0, maximumLengthU);
            double y; size_t ii;
            func.getStats(y, ii);
            EXPECT_TRUE(result.first);
            EXPECT_NEAR(result.second, 4.0, rtSettings.epsilonDu);
            EXPECT_EQ(ii, 4);
            EXPECT_NEAR(y, 0.0, 1e-3);
        };

        void root_finder_tests::dsRootFinder_testNodRobustBisection()
        {
            auto func = nodFunc();
            auto rtSettings = DSrootFinderSettings();
            rtSettings.iterationMethod = DSiterationMethods::DirSamplingIterMethodRobustBisection;
            double z0 = func.GetValue(0.0);
            auto rootFinder = dsRootFinderRobust(rtSettings);
            rootFinder.init(z0);
            auto result = rootFinder.FindRoot(func, 0.0, maximumLengthU);
            double y; size_t ii;
            func.getStats(y, ii);
            EXPECT_TRUE(result.first);
            EXPECT_NEAR(result.second, 4.0, rtSettings.epsilonDu);
            EXPECT_EQ(ii, 14);
            EXPECT_NEAR(y, 0.0, 1e-3);
        };

        // test a situation where z = 0 between 0 and du1
        void root_finder_tests::dsRootFinder_test3()
        {
            auto func = nodFunc();
            auto rtSettings = DSrootFinderSettings();
            rtSettings.iterationMethod = DSiterationMethods::DirSamplingIterMethodRobust;
            rtSettings.du1 = 5.0;
            double z0 = func.GetValue(0.0);
            auto rootFinder = dsRootFinderRobust(rtSettings);
            rootFinder.init(z0);
            auto result = rootFinder.FindRoot(func, 0.0, maximumLengthU);
            double y; size_t ii;
            func.getStats(y, ii);
            EXPECT_TRUE(result.first);
            EXPECT_NEAR(result.second, 4.0, rtSettings.epsilonDu);
            EXPECT_EQ(ii, 3);
            EXPECT_NEAR(y, 0.0, 1e-3);
        };
    }
}

