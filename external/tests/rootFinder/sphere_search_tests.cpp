#include "gtest/gtest.h"
#include "../probMethods/waarts1.h"
#include "sphere_search_tests.h"
#include "../../src/correlation/identity.h"
#include "../../src/distributions/normalDist.h"
#include "../../src/probFuncs/listDistribs.h"
#include "../../src/probMethods/sphere_search.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void sphere_search_tests::all_tests()
        {
            sphere_search_test1();
            sphere_search_test2();
            sphere_search_test3();
            sphere_searchAB_test1();
            sphere_searchAB_test2();
        }

        void sphere_search_tests::sphere_search_test1()
        {
            auto w = waarts1();
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(1.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(2.0, 0.75);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto r = sphere_search(w, s);
            auto u = r.search();
            double zmin; size_t iters;
            r.getStats(zmin, iters);
            ASSERT_EQ(u.size(), 2);
            EXPECT_NEAR(u(0), 0.7071, 0.01);
            EXPECT_NEAR(u(1), 0.0, 0.01);
            EXPECT_EQ(iters, 15);
            EXPECT_NEAR(zmin, 0.06066, 0.001);
        }

        void sphere_search_tests::sphere_search_test2()
        {
            auto w = waarts1();
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(1.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(2.0, 0.75);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto r = sphere_search(w, s);
            auto u = r.search({ 1.0, 0.5 });
            double zmin; size_t iters;
            r.getStats(zmin, iters);
            ASSERT_EQ(u.size(), 2);
            EXPECT_NEAR(u(0), 0.826, 0.01);
            EXPECT_NEAR(u(1), 0.1711, 0.01);
            EXPECT_EQ(iters, 14);
            EXPECT_NEAR(zmin, 0.04698, 0.001);
        }

        void sphere_search_tests::sphere_search_test3()
        {
            auto w = waarts1();
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(1.0, 0.2);
            params.addDistrib(n1);
            auto n2 = new normalDist(2.0, 0.15);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto r = sphere_search(w, s);
            auto u = r.search({ 1.0, 0.5 });
            double zmin; size_t iters;
            r.getStats(zmin, iters);
            ASSERT_EQ(u.size(), 2);
            EXPECT_NEAR(u(0), 4.9193, 0.01);
            EXPECT_NEAR(u(1), 0.0, 0.01);
            EXPECT_EQ(iters, 60);
            EXPECT_NEAR(zmin, 0.01613, 0.001);
        }

        void sphere_search_tests::sphere_searchAB_test1()
        {
            auto w = waarts1();
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(1.0, 0.2);
            params.addDistrib(n1);
            auto n2 = new normalDist(2.0, 0.15);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto r = sphere_searchAB(w, s);
            auto u = r.search({ 1.0, 0.5 });
            double zmin; size_t iters;
            r.getStats(zmin, iters);
            ASSERT_EQ(u.size(), 2);
            EXPECT_NEAR(u(0), 5.0, 0.01);
            EXPECT_NEAR(u(1), 0.0, 0.01);
            EXPECT_EQ(iters, 27);
            EXPECT_NEAR(zmin, 0.0, 0.001);
        }

        void sphere_search_tests::sphere_searchAB_test2()
        {
            auto w = waarts1();
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(1.0, 0.2);
            params.addDistrib(n1);
            auto n2 = new normalDist(2.0, 0.15);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto settings = form_init_settings();
            settings.ABstartZero = true;
            auto r = sphere_searchAB(w, s, settings);
            auto u = r.search({ 1.0, 0.5 });
            double zmin; size_t iters;
            r.getStats(zmin, iters);
            ASSERT_EQ(u.size(), 2);
            EXPECT_EQ(iters, 23);
            EXPECT_NEAR(zmin, 0.0, 0.001);
        }
    }
}
