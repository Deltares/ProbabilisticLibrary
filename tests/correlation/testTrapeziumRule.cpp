#include "testTrapeziumRule.h"
#include "../../src/correlation/trapeziumRule.h"
#include <gtest/gtest.h>
#include <math.h>

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void testTrapeziumRule::allTrapeziumRuleTests()
        {
            trapeziumRuleTest1();
        }

        void testTrapeziumRule::trapeziumRuleTest1()
        {
            const int nx = 1061;
            const double x_upperbound = 16.0;
            const double margin = 1.0e-5;

            //vector x is similar to "variable" in method calculateCorrelationNLmodel
            auto xy = std::vector<xyPair>(nx);
            double factorStapSize = pow(10.0, 1.0 / (double)(nx - 2));
            double step = x_upperbound * (1.0 - factorStapSize) / (1.0 - pow(factorStapSize, (double)(nx - 1)));

            xy[0].x = 0.0;
            xy[1].x = step;
            for (size_t i = 2; i < nx; i++)
            {
                step = step * factorStapSize;
                xy[i].x = xy[i - 1].x + step;
            }

            for (size_t i = 0; i < nx; i++)
            {
                //Calculate standard normal pdf
                xy[i].y = exp(-0.5 * pow(xy[i].x, 2)) / 2.50662827463100;
            }

            double Q = trapeziumRule::calc(xy);
            EXPECT_NEAR(Q, 0.5, margin);
        }
    }
}
