#include "gtest/gtest.h"
#include "gaussian_correlation_tests.h"
#include "../../src/correlation/gaussianCorrelation.h"
#include "../../src/correlation/gaussianCorrelationRobust.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        // test Gaussian correlation model (rho = -1.0, 0.0, 1.0)
        void gaussian_correlation_tests::testCorrelationGaussianModel1a()
        {
            auto rho = { -1.0, 0.0, 1.0 };
            auto expected_u2 = std::vector<double>{ -8.0, 1.0, 8.0 };
            // trivial results based on values of u1 = 8.0 and u2 = 1.0

            int ii = 0;
            vector1D u = vector1D(2);
            for (auto r : rho)
            {
                auto corr = std::vector<corrStruct>();
                corr.push_back({ 0, 1, r });
                auto gs = gaussianCorrelation(2, corr);
                u(0) = 8.0;
                u(1) = 1.0;
                gs.updateU(u);
                EXPECT_NEAR(expected_u2[ii], u(1), margin);
                ii++;
            }
        }

        // test Gaussian correlation model (other rho values)
        void gaussian_correlation_tests::testCorrelationGaussianModel2a()
        {
            auto rho = { -0.999, -0.1, 0.37, 0.50, 0.999 };
            auto expected_u2 = std::vector<double>{ -7.947290, 0.1949874, 3.889032, 4.866025, 8.036710 };
            // pre-computed in Excel for u1 = 8.0 and u2 = 1.0

            int ii = 0;
            vector1D u = vector1D(2);
            for (auto r : rho)
            {
                auto corr = std::vector<corrStruct>();
                corr.push_back({ 0, 1, r });
                auto gs = gaussianCorrelation(2, corr);
                u(0) = 8.0;
                u(1) = 1.0;
                gs.updateU(u);
                EXPECT_NEAR(expected_u2[ii], u(1), margin);
                ii++;
            }
        }

        // test Gaussian correlation model (rho = -1.0, 0.0, 1.0)
        void gaussian_correlation_tests::testCorrelationGaussianModel1b()
        {
            auto rho = { -1.0, 0.0, 1.0 };
            auto expected_u2 = std::vector<double>{ -8.0, 1.0, 8.0 };
            // trivial results based on values of u1 = 8.0 and u2 = 1.0

            int ii = 0;
            vector1D u = vector1D(2);
            for (auto r : rho)
            {
                auto corr = std::vector<corrStruct>();
                corr.push_back({ 0, 1, r });
                auto gs = gaussianCorrelationRobust(2, corr);
                u(0) = 8.0;
                u(1) = 1.0;
                gs.updateU(u);
                EXPECT_NEAR(expected_u2[ii], u(1), margin);
                ii++;
            }
        }

        // test Gaussian correlation model (other rho values)
        void gaussian_correlation_tests::testCorrelationGaussianModel2b()
        {
            auto rho = { -0.999, -0.1, 0.37, 0.50, 0.999 };
            auto expected_u2 = std::vector<double>{ -7.947290, 0.1949874, 3.889032, 4.866025, 8.036710 };
            // pre-computed in Excel for u1 = 8.0 and u2 = 1.0

            int ii = 0;
            vector1D u = vector1D(2);
            for (auto r : rho)
            {
                auto corr = std::vector<corrStruct>();
                corr.push_back({ 0, 1, r });
                auto gs = gaussianCorrelationRobust(2, corr);
                u(0) = 8.0;
                u(1) = 1.0;
                gs.updateU(u);
                EXPECT_NEAR(expected_u2[ii], u(1), margin);
                ii++;
            }
        }
    }
}
