#include "parseAndRunTests.h"
#include "gtest/gtest.h"
#include "waarts1.h"
#include "../../src/probMethods/parseAndRunMethod.h"
#include "../../src/correlation/gaussianCorrelationRobust.h"
#include "../../src/distributions/normalDist.h"
#include "../../src/distributions/parseDistribution.h"
#include "../../src/probFuncs/listDistribs.h"
#include "../../src/distributions/logNormalDist.h"
#include "../../src/distributions/triangleDist.h"
#include "../../src/distributions/truncatedNormal.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void parseAndRunTests::runAllTests()
        {
            parse_run_form_with_correlation();
            test_parse_distributions();
        }

        void parseAndRunTests::parse_run_form_with_correlation()
        {
            auto w = waarts1();
            auto corr = std::vector<corrStruct>();
            corr.push_back({ 0, 1, 0.5 });
            auto gs = gaussianCorrelationRobust(2, corr);
            auto params = listDistribs();
            auto n1 = new normalDist(2.0, 1.0);
            params.addDistrib(n1);
            auto n2 = new normalDist(1.0, 1.0);
            params.addDistrib(n2);
            auto s = stochSettings(gs, params);

            auto bs = basicSettings();
            auto pg = progress(bs.progressInterval);
            auto pr = parseAndRunMethod(bs, pg);
            auto calcResult = pr.parseAndRun(ProbMethod::FORM, w, s);

            ASSERT_EQ(calcResult.convergence, ConvergenceStatus::success);
            EXPECT_NEAR(calcResult.result.getBeta(), -1.0, 2e-6);
        }

        void parseAndRunTests::test_parse_distributions()
        {
            auto params = listDistribs();
            auto pd = parseDistribution();
            auto p0 = std::vector<double>{ 1,2,0,0 };
            auto p1 = std::vector<double>{ 1,2,3,4 };
            std::vector<std::string> names =
            { "name1", "name2", "name3", "name4", "name5" };
            std::vector<std::string> distributions =
            { "normal", "deterministic", "lognormal2", "triangular", "truncatedNormal" };
            for (size_t i = 0; i < names.size(); i++)
            {
                params.addDistrib(pd.parse({ names[i], distributions[i] }, i > 2 ? p1 : p0));
            }

            ASSERT_EQ(params.size(), 5);
            for (size_t i = 0; i < names.size(); i++)
            {
                ASSERT_EQ(params.getDistrib(i)->getName(), names[i]);
                auto ref = pd.activeParameters(pd.parse(distributions[i]));
                ASSERT_EQ(params.getDistrib(i)->getNrParams(), ref);
            }

            const double u = 3.456;
            const double tol = 1e-12;
            auto x0 = params.getDistrib(0)->getValue(u);
            ASSERT_NEAR(x0, p0[0] + u * p0[1], tol);

            auto x1 = params.getDistrib(1)->getValue(u);
            ASSERT_NEAR(x1, 1.0, tol);

            auto x2 = params.getDistrib(2)->getValue(u);
            auto d = logNormalIIDist(p0[0], p0[1], p0[2]);
            auto x2ref = d.getValue(u);
            ASSERT_NEAR(x2, x2ref, tol);

            auto x3 = params.getDistrib(3)->getValue(u);
            auto tri = triangleDist(p1[0], p1[1], p1[2]);
            auto x3ref = tri.getValue(u);
            ASSERT_NEAR(x3, x3ref, tol);

            auto x4 = params.getDistrib(4)->getValue(u);
            auto trunc = truncatedNormal(p1[0], p1[1], p1[2], p1[3]);
            auto x4ref = trunc.getValue(u);
            ASSERT_NEAR(x4, x4ref, tol);
        }
    }
}
