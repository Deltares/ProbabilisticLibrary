#include <iostream>
#include <string>
#include <memory>
#include "gtest/gtest.h"
#include "utils/testutils.h"
#include "../src/probMethods/fdir.h"
#include "../src/probMethods/dsfi.h"
#include "../src/probMethods/crudeMonteCarlo.h"
#include "../src/probMethods/adaptiveImpSampling.h"
#include "../src/probMethods/numint.h"
#include "../src/probMethods/progress.h"
#include "../src/probFuncs/listDistribs.h"
#include "../src/distributions/distrib.h"
#include "../src/distributions/normalDist.h"
#include "../src/correlation/identity.h"
#include "../src/correlation/gaussianCorrelationRobust.h"
#include "../src/rnd/defaultRandomDouble.h"
#include "../src/rnd/mersenne_twister.h"
#include "../src/rnd/george_marsaglia.h"
#include "probMethods/mhwtest.h"
#include "probMethods/waarts1.h"
#include "rootFinder/ray_search_tests.h"
#include "rootFinder/sphere_search_tests.h"
#include "probMethods/parseAndRunTests.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        size_t numcores;

        int main(int argc, char** argv)
        {
            ::testing::InitGoogleTest(&argc, argv);
            //::testing::GTEST_FLAG(filter) = "probmethod.adaptiveImpSamplingTest";
            auto utils = testutils();
            numcores = utils.readNumThreadsFromEnv(true);
            return RUN_ALL_TESTS();
        }

        TEST(probmethod, spheresearchtests)
        {
            auto r = sphere_search_tests();
            r.all_tests();
        }

        TEST(probmethod, raysearchtests)
        {
            auto r = ray_search_tests();
            r.all_tests();
        }

        TEST(probmethod, mhw)
        {
            auto t = mhwtest();
            t.test1();
            t.test2();
            t.test3();
        }

        TEST(probmethod, form_with_gaussian_correlation)
        {
            auto w = waarts1();
            auto FormFlags = FormSettings();
            auto p = progress();
            auto Form = form(FormFlags, p);
            auto corr = std::vector<corrStruct>();
            corr.push_back({ 0, 1, 0.5 });
            auto gs = gaussianCorrelation(2, corr);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(gs, params);

            auto calcResult = Form.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), -1.0, 2e-6);
        }

        TEST(probmethod, form_with_gaussian_correlation_robust)
        {
            auto w = waarts1();
            auto FormFlags = FormSettings();
            auto p = progress();
            auto Form = form(FormFlags, p);
            auto corr = std::vector<corrStruct>();
            corr.push_back({ 0, 1, 0.5 });
            auto gs = gaussianCorrelationRobust(2, corr);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(gs, params);

            auto calcResult = Form.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), -1.0, 2e-6);
        }

        TEST(probmethod, parse_run_tests)
        {
            auto pr = parseAndRunTests();
            pr.runAllTests();
        }

        TEST(probmethod, cm_test)
        {
            auto w = waarts1();
            auto CMflags = CMsettings();
            auto rnd = defaultRandomDouble(2);
            auto p = progress();
            auto CM = crudeMonteCarlo(CMflags, rnd, p);
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto calcResult = CM.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
#ifdef _MSC_VER
            EXPECT_NEAR(calcResult.result.getBeta(), -0.704965, 2e-6);
#else
            EXPECT_NEAR(calcResult.result.getBeta(), -0.707554, 2e-6);
#endif
        }

        TEST(probmethod, ni_test)
        {
            auto w = waarts1();
            auto NIflags = NIsettings();
            auto p = progress();
            auto NI = numint(NIflags, numcores, p);
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto calcResult = NI.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), -0.699751, 2e-6);
            EXPECT_NEAR(calcResult.result.getAlphaI(0), -0.715474, 2e-6);
            EXPECT_NEAR(calcResult.result.getAlphaI(1), 0.698639, 2e-6);
        }

        TEST(probmethod, form_test)
        {
            auto w = waarts1();
            auto FormFlags = FormSettings();
            auto p = progress();
            auto Form = form(FormFlags, p);
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto calcResult = Form.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), -0.707107, 2e-6);
        }

        TEST(probmethod, fdir_test)
        {
            auto w = waarts1();
            auto DSflags = DSsettings();
            auto rtSettings = DSrootFinderSettings();
            auto FormFlags = FormSettings();
            auto rnd = george_marsaglia(2, 2);
            auto p = progress();
            auto rtf = dsRootFinder(rtSettings);
            auto Fdir = fdir(DSflags, FormFlags, rnd, rtf, p);
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto calcResult = Fdir.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), -0.707107, 2e-6);
        }

        TEST(probmethod, ds_test)
        {
            auto w = waarts1();
            auto DSflags = DSsettings();
            auto rtSettings = DSrootFinderSettings();
            auto rnd = george_marsaglia(2, 2);
            auto rtf = dsRootFinder(rtSettings);
            auto p = progress();
            auto DS = directionalSampling(DSflags, rnd, rtf, numcores, p);
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto calcResult = DS.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), -0.706577, 2e-6);
        }

        TEST(probmethod, ds_gm_test)
        {
            auto w = waarts1();
            auto DSflags = DSsettings();
            auto rtSettings = DSrootFinderSettings();
            auto rnd = george_marsaglia(234, 456);
            auto rtf = dsRootFinder(rtSettings);
            auto p = progress();
            auto DS = directionalSampling(DSflags, rnd, rtf, numcores, p);
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto calcResult = DS.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), -0.707644, 2e-6);
        }

        TEST(probmethod, ds_test_threadsave)
        {
            auto w = waarts1();
            auto DSflags = DSsettings();
            auto rtSettings = DSrootFinderSettings();
            auto rnd = mersenne_twister(234);
            auto rtf = dsRootFinder(rtSettings);
            auto p = progress();
            auto DS = directionalSampling(DSflags, rnd, rtf, numcores, p);
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto calcResult = DS.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), -0.707768, 2e-6);
            EXPECT_NEAR(calcResult.result.getAlphaI(0), -0.707, 1e-3);
            EXPECT_NEAR(calcResult.result.getAlphaI(1), 0.707, 1e-3);
        }

        TEST(probmethod, dsfi_test)
        {
            auto w = waarts1();
            auto DSflags = DSsettings();
            auto rtSettings = DSrootFinderSettings();
            auto FormFlags = FormSettings();
            auto rnd = george_marsaglia(2, 2);
            auto p = progress();
            auto rtf = dsRootFinder(rtSettings);
            auto DSFI = dsfi(DSflags, FormFlags, rnd, rtf, p);
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto calcResult = DSFI.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), -0.706577, 2e-6);
        }

        TEST(probmethod, im_test)
        {
            auto w = waarts1(numcores);
            auto IMflags = IMsettings();
            IMflags.minimumSamples = 18;
            IMflags.maximumSamples = 18;
            IMflags.chunkSize = 10;
            auto rnd = george_marsaglia(2, 2);
            auto p = progress();
            auto bf = IMbuffer(IMflags.chunkSize, 2, true);
            std::unique_ptr<lsf> IM(numcores > 1 ?
                (lsf*) new ImportanceSampling(IMflags, rnd, p, bf) :
                (lsf*) new ImportanceSamplingSeq(IMflags, rnd, p));
            auto id = identity(2);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(id, params);

            auto calcResult = IM->calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::noConvergence);
            EXPECT_NEAR(calcResult.result.getBeta(), -1.670572, 2e-6);
            EXPECT_NEAR(calcResult.result.getAlphaI(0), -0.978999, 2e-6);
            EXPECT_NEAR(calcResult.result.getAlphaI(1), -0.203867, 2e-6);
        }

        // Test for importance sampling.
        // A simple Z function is called in which the failure probability can be analytically computed.
        // The returned beta value is then compared with the (in Matlab-computed) known beta.
        // The test passes if the difference in beta is less than 0.
        TEST(probmethod, importanceSamplingTest)
        {
            // const double betaKnown = 2.82842712474619;

            // Initialize stochast data
            auto stoch1 = new uSpace();
            auto stoch2 = new uSpace();
            auto stoch3 = new deterministicDist(4.0);

            auto id = identity(3);
            auto params = listDistribs();
            params.addDistrib(stoch1);
            params.addDistrib(stoch2);
            params.addDistrib(stoch3);
            auto s = stochSettings(id, params, true);

            auto w = simple();
            auto IMflags = IMsettings();
            auto rnd = george_marsaglia(2, 2);
            auto p = progress();
            auto bf = IMbuffer(IMflags.chunkSize, 2, true);
            auto IM = ImportanceSampling(IMflags, rnd, p, bf);

            auto calcResult = IM.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            auto ref = alphaBeta(2.806227, { -0.759721, -0.650249 });
            auto utils = testutils();
            utils.checkAlphaBeta(calcResult.result, ref, 1e-6);
        }

        // Test for adaptive importance sampling.
        // A simple Z function is called in which the failure probability can be analytically computed.
        // The returned beta value is then compared with the (in Matlab-computed) known beta.
        // The test passes if the difference in beta is less than 0.
        TEST(probmethod, adaptiveImpSamplingTest)
        {
            // const double betaKnown = 2.82842712474619;

            // Initialize stochast data
            auto stoch1 = new uSpace();
            auto stoch2 = new uSpace();
            auto stoch3 = new deterministicDist(4.0);

            auto id = identity(3);
            auto params = listDistribs();
            params.addDistrib(stoch1);
            params.addDistrib(stoch2);
            params.addDistrib(stoch3);
            auto s = stochSettings(id, params, true);

            auto w = simple();
            auto AIMflags = AIMsettings();
            AIMflags.startMethod = StartMethods::RaySearch;
            AIMflags.nAdp = 10;
            AIMflags.maximumSamples = 10000;
            AIMflags.maxSamplesDef = 50000;
            auto rnd = george_marsaglia(2, 2);
            auto p = progress();
            for (int i = 0; i < 2; i++)
            {
                auto IM = adaptiveImpSampling(AIMflags, rnd, p, i == 0);
                rnd.restart();
                auto calcResult = IM.calc(w, s);
                calcResult.printResult(std::cout, s);
                ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
                auto ref = alphaBeta(2.806226770, { -0.759721, -0.6502493 });
                auto utils = testutils();
                utils.checkAlphaBeta(calcResult.result, ref, 1e-2);
            }
        }

        TEST(probmethod, cmDeterministicTest)
        {
            // Initialize stochast data
            auto stoch1 = new uSpace();
            auto stoch2 = new uSpace();
            auto stoch3 = new deterministicDist(2.0);

            auto id = identity(3);
            auto params = listDistribs();
            params.addDistrib(stoch1);
            params.addDistrib(stoch2);
            params.addDistrib(stoch3);
            auto s = stochSettings(id, params, true);

            auto w = simple();
            auto CMflags = CMsettings();
            auto rnd = george_marsaglia(2, 2);
            auto p = progress();
            auto CM = crudeMonteCarlo(CMflags, rnd, p);

            auto calcResult = CM.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::noConvergence);
            auto ref = alphaBeta(1.412346, { -0.719827, -0.694154 });
            auto utils = testutils();
            utils.checkAlphaBeta(calcResult.result, ref, 1e-6);
        }
    }
}
