#include "gtest/gtest.h"
#include "../probMethods/waarts1.h"
#include "ray_search_tests.h"
#include "../../src/correlation/identity.h"
#include "../../src/distributions/normalDist.h"
#include "../../src/probFuncs/listDistribs.h"
#include "../../src/probMethods/ray_search.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void ray_search_tests::all_tests()
        {
            ray_search_test1();
            ray_search_test2();
            ray_search_test3();
        }

        void ray_search_tests::ray_search_test1()
        {
            auto w = waarts1();
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(1.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(2.0, 0.75);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto formConsts = form_init_settings();
            formConsts.maxu = 10.0;

            auto r = ray_search(w, s, formConsts);
            auto u = r.search();
            double zmin; size_t iters;
            r.getStats(zmin, iters);
            ASSERT_EQ(u.size(), 2);
            ASSERT_EQ(u(0), u(1));
            ASSERT_NEAR(u(0), 4.0, 0.001);
            ASSERT_EQ(iters, 15);
            ASSERT_NEAR(zmin, -0.075, 1e-9);
        }

        void ray_search_tests::ray_search_test2()
        {
            auto w = waarts1();
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(1.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(2.0, 0.75);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto formConsts = form_init_settings();
            formConsts.maxu = 10.0;

            auto r = ray_search(w, s, formConsts);
            auto u = r.search({ 1.0, 0.5 });
            double zmin; size_t iters;
            r.getStats(zmin, iters);
            ASSERT_EQ(u.size(), 2);
            ASSERT_NEAR(u(0), 1.6, 0.001);
            ASSERT_NEAR(u(1), 0.8, 0.001);
            ASSERT_EQ(iters, 7);
            ASSERT_NEAR(zmin, -0.1875, 1e-9);
        }

        void ray_search_tests::ray_search_test3()
        {
            auto w = waarts1();
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(1.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(2.0, 0.75);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto r = ray_searchAB(w, s);
            auto u = r.search({ 1.0, 0.5 });
            double zmin; size_t iters;
            r.getStats(zmin, iters);
            ASSERT_EQ(u.size(), 2);
            ASSERT_NEAR(u(0), 1.6, 0.001);
            ASSERT_NEAR(u(1), 0.8, 0.001);
            ASSERT_EQ(iters, 3);
            ASSERT_NEAR(zmin, 0.0, 1e-9);
        }
    }
}
