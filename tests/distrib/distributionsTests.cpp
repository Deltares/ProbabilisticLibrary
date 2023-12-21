#include <math.h>
#include "../../src/extern/asa063.hpp"
#include "distributionsTests.h"
#include "../../src/distributions/distrib.h"
#include "gtest/gtest.h"
#include "../utils/testdata.h"
#include "../../src/distributions/distBeta.h"
#include "../../src/distributions/gumble.h"
#include "../../src/distributions/weibull.h"
#include "../../src/distributions/logNormalDist.h"
#include "../../src/distributions/uniformDist.h"
#include "../../src/distributions/mtable.h"
#include "../../src/distributions/combine_x1x2_mtable.h"
#include "../../src/distributions/truncatedNormal.h"
#include "../../src/distributions/triangleDist.h"
#include "../../src/distributions/paretoDist.h"
#include "../../src/distributions/rayleighDist.h"
#include "../../src/distributions/exponentialDist.h"
#include "../../src/probFuncs/conversions.h"

using namespace Deltares::ProbLibCore;
using namespace ASA::conversions;

namespace Deltares
{
    namespace ProbLibTests
    {

        void distributionsTests::allTests()
        {
            betaInverseTestActualData();
            uniform_dist_test();
            logNormalInverseTestProbOneHalf();
            logNormalInverseTypeIITestProbOneHalf();
            logNormalTypeIITest();
            weibullTest();
            gumbelInverseTest();
            gumbelIIandModifiedTests();
            betaDistTest1();
            betaDistTest2();
            TruncatedNormalTests();
            TriangularInverseTestProbZero();
            TriangularInverseTestProbOne();
            TriangularInverseTestProbContinuous();
            testPareto();
            rayleigh5000DistributionTest();
            exponentialTestProbZero();
        }

        void distributionsTests::uniform_dist_test()
        {
            auto u = uniformDist(1.0, 2.0);
            auto x1 = u.getValue(-2.0);
            auto x2 = u.getValue(0.0);
            auto x3 = u.getValue(2.0);
            EXPECT_NEAR(x1, 1.0227501, 1e-6);
            EXPECT_NEAR(x2, 1.5, 1e-10);
            EXPECT_NEAR(x3, 1.9772499, 1e-6);
        }

        // Test logNormalInverse where a known value should be returned
        void distributionsTests::logNormalInverseTestProbOneHalf()
        {
            double mu = 2.0;
            double sig = 1.0;
            double par3 = 0;
            double u = 0.0;
            auto lognorm = logNormalDist(mu, sig, par3);

            double x = lognorm.getValue(u);

            EXPECT_NEAR(exp(mu), x, 1e-9);
        }

        // Test logNormalInverse Type II where a known value should be returned
        // output should be equal to exp(par1) for non-exceedance probability p = 0.5
        void distributionsTests::logNormalInverseTypeIITestProbOneHalf()
        {
            double mu = 2.0;                                      // mu, mean of log-transformed variable
            double sig = 1.0;                                      // sig, standard deviation of log-transformed variable
            double M = exp(mu + sig * sig / 2);                        // par1, mean of actual variable
            double S = sqrt((exp(sig * sig) - 1) * exp(2 * mu + sig * sig)); // par2, standard deviation of actual variable
            double par3 = 0;                                        // par3, horizontal shift
            double u = 0.0;                                      // u value, equivalent to a probability of 0.5

            auto lognorm2 = logNormalIIDist(M, S, par3);
            double x = lognorm2.getValue(u);

            EXPECT_NEAR(exp(mu), x, 1e-9);
        }

        // Test logNormal Type II where a known value should be returned
        // output is fixed with results from the code itself
        void distributionsTests::logNormalTypeIITest()
        {
            const size_t nrTests = 10;
            auto u = std::vector<double>({ -4.30258509299405,
              -3.60943791243410, -3.20397280432594, -2.91629073187416,
              -2.69314718055995, -2.51082562376599, -2.35667494393873,
              -2.22314355131421, -2.10536051565783, -2.00000000000000 });

            double mu = 2.0;                                      // mu, mean of log-transformed variable
            double sig = 1.0;                                      // sig, standard deviation of log-transformed variable
            double M = exp(mu + sig * sig / 2);                        // par1, mean of actual variable
            double S = sqrt((exp(sig * sig) - 1) * exp(2 * mu + sig * sig)); // par2, standard deviation of actual variable
            double par3 = 0;                                        // par3, horizontal shift

            auto lognorm2 = logNormalIIDist(M, S, par3);
            for (size_t i = 1; i <= nrTests; i++)
            {
                double x = 0.1 * (double)i;
                double y = lognorm2.getValue(u[i - 1]);
                EXPECT_NEAR(x, y, 1e-9);
            }
        }

        // based on weibInverseMatlabTest
        void distributionsTests::weibullTest()
        {
            auto u = std::vector<double>{ -2.4024024024024374e-2, -8.0080080080078934e-3, 8.0080080080087278e-3, 2.4024024024024374e-2 };
            auto a = std::vector<double>{ 5.4864864864864868, 5.4954954954954953, 5.5045045045045047, 5.5135135135135132 };
            auto b = std::vector<double>{ 1.9969969969969970, 1.9989989989989989, 2.0010010010010011, 2.0030030030030028 };
            auto ML = std::vector<double>{ 4.5034743854960908, 4.5538028096390359, 4.6043531106325606, 4.6551231144729401 };

            for (size_t i = 0; i < u.size(); i++)
            {
                auto w = weibull(a[i], b[i], 0.0);
                double x = w.getValue(u[i]);
                ASSERT_NEAR(x, ML[i], 1e-9);
            }

            auto cw = conditionalWeibull(1.0, 2.0, 3.0, 4.0);
            double x = cw.getValue(0.0);
            ASSERT_NEAR(x, 3.27914734065, 1e-9);
        }

        void distributionsTests::betaDistTest1()
        {
            //  Location parameters in present beta distribution:
            double a = -2.0;
            double b = 6.0;
            //  Shape parameters in present beta distribution:
            double alpha = 3.0;
            double beta = alpha;         // symmetric distribution
            double p = 0.5;           // Probability of non-exceedence
            double q = 1.0 - p;       // Corresponding Probability of exceedence
            double tolDiff = 1.0e-7;
            double expectedValue = 0.5 * (a + b);   // because of symmetry due to alpha= beta

            auto bd = distBeta(a, b, alpha, beta);
            double computedValue = bd.distributionBetaInvPQ(p, q);
            ASSERT_NEAR(computedValue, expectedValue, tolDiff);
        }

        // Test gumbelInverse where a known value should be returned
        // output should be equal to rate parameter (par1) for p = 1/exp(1)
        void distributionsTests::gumbelInverseTest()
        {
            double margin = 0.000001;      // margin for comparing output with known value
            double b = 2.0;          // par1, rate parameter (1/scale)
            double a = 1.0;          // par2, location parameter
            double p = 1.0 / exp(1.0);  // non-exceedance probability
            double q = 1 - p;           // exceedance probability
            double u = conversions::betaFromQ(q);
            auto g = gumbel(a, b);
            double x = g.getValue(u);

            ASSERT_NEAR(a, x, margin);
        }

        void distributionsTests::gumbelIIandModifiedTests()
        {
            auto gII = gumbelII(1.0, 2.0);
            double x = gII.getValue(-1.0);
            ASSERT_NEAR(x, -0.8518365467885, 1e-9);
            x = gII.getValue(0.0);
            ASSERT_NEAR(x, 0.67143148848528, 1e-9);
            x = gII.getValue(1.0);
            ASSERT_NEAR(x, 2.83801400659, 1e-9);

            auto gMod = gumbelModified(1.0, 2.0, 0.0);
            x = gMod.getValue(-1.0);
            ASSERT_NEAR(x, -0.375756985929, 1e-9);
            x = gMod.getValue(0.0);
            ASSERT_NEAR(x, 0.16897943548, 1e-9);
            x = gMod.getValue(1.0);
            ASSERT_NEAR(x, 0.6600867269, 1e-9);

            auto gTrunc = gumbelTruncatedModified(1.0, 2.0, 0.0, 0.5);
            x = gTrunc.getValue(-1.0);
            ASSERT_NEAR(x, -0.7351711946, 1e-9);
            x = gTrunc.getValue(0.0);
            ASSERT_NEAR(x, -0.1794113454, 1e-9);
            x = gTrunc.getValue(1.0);
            ASSERT_NEAR(x, 0.069572224, 1e-9);
        }

        void distributionsTests::betaDistTest2()
        {
            //  Location parameters in present beta distribution:
            double b = 2.0;
            double a = -b;
            //  Shape parameters in present beta distribution:
            double alpha = 2.0;
            double beta = alpha;            // alpha= beta= 2 ==> symmetric parabolic density function f(x|b)= 3/(4b) * ( 1 - (x/b)^2)
            //  For this setting: CDF F(x|b)= 1/2 + 3/4*(x/b) - 1/4*(x/b)^3 for -b<x<b
            //  Chose expected X-value for the inversion of beta distribution
            double expectedXValue = 0.25 * b;
            //  Computed corresponding and analytically exact probability of non exceedence p
            double expectedPValue = 0.5 + 0.75 * expectedXValue / b - 0.25 * pow(expectedXValue / b, 3);
            //  ----------------------------------------------------------------------------
            //  Entry to the tests using the expected X and P-values
            double tolDiff = 1.0e-7;
            //  ----------------------------------------------------------------------------
            //  Test on the CDF of the beta distribution. This is thus a test for function betain(), that provides the CDF of the standard beta-distribution.
            double x = (expectedXValue - a) / (b - a);     // Normalisation of X to standard beta distribution
            double lnComplBetaFunc = -log(6.0);                       // Complete beta-function is 6 for alpha= beta= 2
            int ifault;
            double computedPValue = betain(x, alpha, beta, lnComplBetaFunc, ifault);
            //  Compare computed and expected result
            ASSERT_NEAR(computedPValue, expectedPValue, tolDiff);
            //  ----------------------------------------------------------------------------
            //  Test on the inversion of the CDF of the beta distribution
            double p = expectedPValue;
            double q = 1.0 - p;        // Corresponding probability of exceedence

            auto bd = distBeta(a, b, alpha, beta);
            double computedXValue = bd.distributionBetaInvPQ(p, q);

            //computedXValue= betaInverse( a, b, alpha, beta, p, q);
        //  Compare computed and expected result
            ASSERT_NEAR(computedXValue, expectedXValue, tolDiff);
        }

        std::vector<double[7]> distributionsTests::refValues()
        {
            auto data = std::vector<double>{
             5577.570,   -314.519,      9.259,    340.882,    679.751,   1025.223,   1376.628,
             5578.570,   -314.519,      9.259,    340.882,    679.751,   1025.223,   1376.628,
             6615.120,   -343.652,     20.852,    392.204,    769.594,   1152.175,   1539.071,
             7647.690,   -383.985,     41.115,    472.183,    907.991,   1347.272,   1788.740,
             8707.180,   -425.213,     76.614,    582.624,   1090.864,   1599.362,   2106.151,
             9732.930,   -448.343,    108.379,    665.761,   1221.203,   1772.139,   2316.058,
            11185.320,   -615.190,     73.513,    754.207,   1422.109,   2072.670,   2701.657,
            13223.490,   -916.264,   -139.876,    609.999,   1326.661,   2004.219,   2637.732,
            14114.880,   -892.957,   -128.554,    600.539,   1287.970,   1928.429,   2517.790,
            14642.090,  -1004.970,    -90.070,    764.714,   1548.962,   2254.901,   2877.705,
            15354.040,  -1274.562,    -94.221,    964.067,   1880.435,   2644.505,   3255.843,
            16243.820,  -1630.207,   -297.642,    845.826,   1776.248,   2489.481,   3000.497,
            16876.690,  -1783.380,   -406.497,    733.884,   1616.701,   2248.852,   2662.206,
            17230.180,  -1759.889,   -389.536,    716.509,   1542.538,   2105.489,   2449.657,
            25000.000,  -5000.000,  -5000.000,  -5000.000,  -5000.000,  -5000.000,  -5000.000,
            25001.000,  -5001.000,  -5001.000,  -5001.000,  -5001.000,  -5001.000,  -5001.000 };
            auto ref = std::vector<double[7]>(16);
            size_t ii = 0;
            for (size_t i = 0; i < 16; i++)
            {
                for (size_t j = 0; j < 7; j++)
                {
                    ref[i][j] = data[ii];
                    ii++;
                }
            }
            return ref;
        }

        void distributionsTests::betaInverseTestActualData()
        {
            const double margin = 0.0015;

            auto su = testdata::getStatUncBeta();
            auto sut = mtable(su, extrapolation::smartLinear, 4);
            auto db = distBeta4p();
            auto cmb = combine_x1x2_mtable(sut, db);
            auto allRefs = refValues();
            for (size_t i = 0; i < 16; i++)
            {
                for (size_t j = 1; j < 7; j++)
                {
                    double u = (double)j;
                    auto calculated = cmb.getx(su(i, 0), u - 2.0);
                    auto ref = allRefs[i][j] + allRefs[i][0];
                    ASSERT_NEAR(calculated, ref, margin);
                }
            }
        }

        // test for distributed type truncated normal
        void distributionsTests::TruncatedNormalTests()
        {
            //
            // for negative large u, minimum is expected
            //
            double u = -9999.0;
            double mean = 1.0;
            double deviation = 1.0;
            double minimum = 0.1;
            double maximum = 9.9;
            auto tn = truncatedNormal(mean, deviation, minimum, maximum);
            double x = tn.getValue(u);
            ASSERT_NEAR(x, minimum, 1e-5);

            //
            // for positive large u, maximum is expected
            //
            u = 9999.0;
            x = tn.getValue(u);
            ASSERT_NEAR(x, maximum, 1e-5);

            //
            // with minimum and maximum resp. -huge, +huge, distribution is equal to normal distribution
            //
            minimum = -DBL_MAX;
            maximum = DBL_MAX;
            auto tn2 = truncatedNormal(mean, deviation, minimum, maximum);
            for (int i = -25; i <= 25; i++)
            {
                u = 0.1 * (double)i;
                x = tn2.getValue(u);
                double expected = mean + u * deviation;
                ASSERT_NEAR(x, expected, 1e-5);
            }
        }

        // inverse triangular test: tests the value of x for a probability equal to zero
        void distributionsTests::TriangularInverseTestProbZero()
        {
            double a = 2.0;
            double b = 5.0;
            double c = 10.0;
            double p = 0.0;
            double q = 1.0 - p;
            double u = conversions::betaFromQ(q);

            auto td = triangleDist(a, b, c);
            double x = td.getValue(u);

            ASSERT_NEAR(a, x, 1e-9);
        }

        // inverse triangular test: tests the value of x for a probability equal to one
        void distributionsTests::TriangularInverseTestProbOne()
        {
            double a = 2.0;
            double b = 5.0;
            double c = 10.0;
            double p = 1.0;
            double q = 1.0 - p;
            double u = conversions::betaFromQ(q);

            auto td = triangleDist(a, b, c);
            double x = td.getValue(u);

            ASSERT_NEAR(c, x, 1e-9);
        }

        // inverse triangular test: tests the continuity around the value x = b
        void distributionsTests::TriangularInverseTestProbContinuous()
        {
            double a = 2.0;
            double b = 5.0;
            double c = 10.0;

            // test with probability for x slightly greater than b
            double p = (b - a) / (c - a) + 10e-10;
            double q = 1.0 - p;
            double u = conversions::betaFromQ(q);

            auto td = triangleDist(a, b, c);
            double x1 = td.getValue(u);

            // test with probability for x slightly less than than b
            p = (b - a) / (c - a) - 10e-10;
            q = 1.0 - p;
            u = conversions::betaFromQ(q);
            double x2 = td.getValue(u);

            ASSERT_NEAR(x1, x2, 1e-7);
        }

        void distributionsTests::testPareto()
        {
            const auto uArr = std::vector<double>{ -4.0040040040040005e-2, -2.4024024024024374e-2,
        -8.0080080080078934e-3, 8.0080080080087278e-3, 2.4024024024024374e-2, 4.0040040040040012e-2 };
            const auto kArr = std::vector<double>{ 9.9674242643221000e-2, -2.2143310674059688e-1,
        -2.4275219011408047e-1, -2.9108139752188883e-1, -1.2708399264655029e-1, 1.9003872806202338e-1 };
            const auto sArr = std::vector<double>{ 2.3322474754774034, 1.2618896441980103,
        1.1908260329530651, 1.0297286749270373, 1.5763866911781657, 2.6334624268734110 };
            const auto lArr = std::vector<double>{ 3.3306186886935087e+0, 6.5472411049502588e-1,
        4.7706508238266265e-1, 7.4321687317593157e-2, 1.4409667279454141e+0, 4.0836560671835285e+0 };
            const auto MLarr = std::vector<double>{ 4.9259159150409850e+0, 1.4449890212344032e+0,
        1.2303607906527065e+0, 7.2606825991825930e-1, 2.5147923319803507e+0, 6.1325330721189806e+0 };
            const auto MLzero = std::vector<double>{ 4.8738850860145710e+0, 1.5054421454941462e+0,
        1.2948983329250026e+0, 7.9467567006610373e-1, 2.5641419212659180e+0, 5.9945152239935764e+0 };

            for (size_t i = 0; i < uArr.size(); i++)
            {
                auto d1 = paretoDist(lArr[i], sArr[i], kArr[i]);
                double x1 = d1.getValue(uArr[i]);
                ASSERT_NEAR(x1, MLarr[i], 1e-4);
                auto d2 = paretoDist(lArr[i], sArr[i], 0.0);
                double x2 = d2.getValue(uArr[i]);
                ASSERT_NEAR(x2, MLzero[i], 1e-4);
            }
        }

        // check that rayleighNInverse is always finite for large u and N = 5000
        // (as used for parameter 'Ratio of H_max,0 to H_S for N waves m_Ray' in structural failure)
        void distributionsTests::rayleigh5000DistributionTest()
        {
            const auto references = std::vector<double>{
               1.57589, 1.57943, 1.58300, 1.58672, 1.59051, 1.59434, 1.59820, 1.60209,
               1.60602, 1.60999, 1.61399, 1.61804, 1.62213, 1.62625, 1.63042, 1.63465,
               1.63890, 1.64321, 1.64756, 1.65196, 1.65641, 1.66092, 1.66547, 1.67007,
               1.67471, 1.67944, 1.68419, 1.68903, 1.69392, 1.69887, 1.70388, 1.70895,
               1.71410, 1.71931, 1.72459, 1.72994, 1.73537, 1.74088, 1.74646, 1.75213,
               1.75788, 1.76372, 1.76964, 1.77566, 1.78178, 1.78799, 1.79431, 1.80073,
               1.80726, 1.81390, 1.82066, 1.82754, 1.83454, 1.84167, 1.84894, 1.85634,
               1.86389, 1.87159, 1.87945, 1.88746, 1.89565, 1.90401, 1.91255, 1.92127,
               1.93020, 1.93933, 1.94867, 1.95823, 1.96802, 1.97806, 1.98834, 1.99889,
               2.00971, 2.02081, 2.03221, 2.04392, 2.05595, 2.06832, 2.08103, 2.09411,
               2.10757, 2.12143, 2.13569, 2.15038, 2.16550, 2.18109, 2.19714, 2.21369,
               2.23073, 2.24830, 2.26639, 2.28503, 2.30422, 2.32398, 2.34432, 2.36524,
               2.38675, 2.40886, 2.43157, 2.45488, 2.47879, 2.50331, 2.52842, 2.55412,
               2.58040, 2.60727, 2.63470, 2.66269, 2.69123, 2.72032, 2.74992, 2.78004,
               2.81067, 2.84178, 2.87336, 2.90542, 2.93792, 2.97089, 3.00426, 3.03805,
               3.07224, 3.10682, 3.14177, 3.17710, 3.21279, 3.24882, 3.28519, 3.32186,
               3.35888, 3.39620, 3.43382, 3.47172, 3.50990, 3.54836, 3.58708, 3.62606,
               3.66528, 3.70475, 3.74445, 3.78436, 3.82452, 3.86489, 3.90546, 3.94623,
               3.98720, 4.02836, 4.06970, 4.11123, 4.15293, 4.19480, 4.23683, 4.27903,
               4.32138, 4.36388, 4.40654, 4.44933, 4.49227, 4.53534, 4.57854, 4.62187,
               4.66533 };

            const int N = 5000;
            const double sigma = 0.5;

            const int umax = 8;
            const int nn = 10;   // steps of 0.1, nn = 1 / 0.1
            const int length = nn * umax;

            auto d = rayleighNDist(sigma, N);
            for (int i = -length; i <= length; i++)
            {
                double u = ((double)i) / (double)nn;
                double x = d.getValue(u);
                ASSERT_NEAR(x, references[i + length], 1e-4);
            }
        }

        // Inverse exponential test: This tests if the second parameter is returned for a probability equal to zero
        void distributionsTests::exponentialTestProbZero()
        {
            const double p = 0;
            const double q = 1.0 - p;
            const double u = conversions::betaFromQ(q);
            const double a = 2;
            const double b = 3;

            auto d = exponentialDist(a, b);
            double x = d.getValue(u);

            ASSERT_NEAR(b, x, 1e-9);
        }
    }
}
