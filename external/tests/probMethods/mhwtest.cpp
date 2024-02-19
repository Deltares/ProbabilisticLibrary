#include <iostream>
#include "mhwtest.h"
#include <gtest/gtest.h>
#include "../../src/probMethods/form.h"
#include "../../src/probFuncs/listDistribs.h"
#include "../../src/probFuncs/stochSettings.h"
#include "../../src/distributions/distBeta.h"
#include "../../src/distributions/normalDist.h"
#include "../../src/correlation/identity.h"
#include "../../src/distributions/mtable.h"
#include "../../src/distributions/combine_x1x2_mtable.h"
#include "../utils/testdata.h"
#include "../utils/testutils.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        double mhwtestz::zfunc(const probDraw& d) const
        {
            return refValue - d.getx(0);
        }

        void mhwtest::test1() const
        {
            auto qu = testdata::getTable();
            auto w = mhwtestz(16000.0);
            auto FormFlags = FormSettings();
            FormFlags.relaxationFactor = 0.7;
            FormFlags.du = 0.1;
            auto p = progress();
            auto Form = form(FormFlags, p);
            auto id = identity(1);
            auto params = listDistribs();
            auto myTable = new dtable(qu, extrapolation::smartLinear, 1);
            params.addDistrib(myTable);
            auto s = stochSettings(id, params);

            auto calcResult = Form.calc(w, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), 4.047, 1e-3);
        }

        void mhwtest::test2() const
        {
            auto qu = testdata::getTable();
            auto w = mhwtestz(16000.0);
            auto FormFlags = FormSettings();
            FormFlags.relaxationFactor = 0.7;
            FormFlags.du = 0.1;
            auto p = progress();
            auto Form = form(FormFlags, p);
            auto id = identity(3);
            auto myTable = new dtable(qu, extrapolation::smartLinear, 1);
            auto su = testdata::getStatUnc();
            auto sut = mtable(su, extrapolation::smartLinear, 2);
            auto temp = normalDist4p();
            auto cmb = new combine_x1x2_mtable(sut, temp, 1, 2);
            auto params = listDistribs();
            params.addDistrib(cmb);
            params.addDistrib(myTable);
            auto u = new uSpace();
            params.addDistrib(u);
            auto s = stochSettings(id, params, true);

            auto calcResult = Form.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), 3.904, 1e-3);
            EXPECT_NEAR(calcResult.result.getAlphaI(0), -0.9722, 1e-4);
            EXPECT_NEAR(calcResult.result.getAlphaI(1), -0.2343, 1e-4);
        }

        void mhwtest::test3() const
        {
            auto qu = testdata::getTable();
            auto utils = testutils();
            auto numcores = utils.readNumThreadsFromEnv(false);
            auto w = mhwtestz(16000.0, numcores);
            auto FormFlags = FormSettings();
            FormFlags.relaxationFactor = 0.9;
            FormFlags.du = 0.3;
            auto p = progress();
            auto Form = form(FormFlags, p);
            auto id = identity(3);
            auto myTable = new dtable(qu, extrapolation::smartLinear, 1);
            auto su = testdata::getStatUncBeta();
            auto sut = mtable(su, extrapolation::basicLinear, 4);
            auto db = distBeta4p();
            auto cmb = new combine_x1x2_mtable(sut, db, 1, 2);
            auto params = listDistribs();
            params.addDistrib(cmb);
            params.addDistrib(myTable);
            auto u = new uSpace();
            params.addDistrib(u);
            auto s = stochSettings(id, params, true);

            auto calcResult = Form.calc(w, s);
            calcResult.printResult(std::cout, s);
            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), 3.864, 1e-3);
            EXPECT_NEAR(calcResult.result.getAlphaI(0), -0.9585, 1e-4);
            EXPECT_NEAR(calcResult.result.getAlphaI(1), -0.2851, 1e-4);
        }
    }
}
