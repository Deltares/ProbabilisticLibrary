#include <cmath>
#include "gtest/gtest.h"
#include "matmul_tests.h"
#include "../../src/utils/matrix.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void matmul_tests::matmul_test1()
        {
            auto m1 = Matrix(3, 3);
            auto m2 = Matrix(3, 2);
            auto m3 = Matrix(3, 2);
            m1(0, 0) = 0.2;
            m1(0, 1) = 0.8;
            m1(0, 2) = 1.1;
            m1(1, 0) = 0.0;
            m1(1, 1) = 0.2;
            m1(1, 2) = 0.5;
            m1(2, 0) = 1.8;
            m1(2, 1) = 1.6;
            m1(2, 2) = 1.4;
            m2(0, 0) = 50.0;
            m2(0, 1) = 60.0;
            m2(1, 0) = 83.0;
            m2(1, 1) = 55.0;
            m2(2, 0) = 32.0;
            m2(2, 1) = 45.0;
            m3(0, 0) = 111.6;
            m3(0, 1) = 105.5;
            m3(1, 0) = 32.6;
            m3(1, 1) = 33.5;
            m3(2, 0) = 267.6;
            m3(2, 1) = 259.0;
            auto m4 = m1.matmul(m2);
            for (size_t i = 0; i < 3; i++)
            {
                for (size_t j = 0; j < 2; j++)
                {
                    EXPECT_NEAR(m3(i, j), m4(i, j), margin);
                }
            }
        }

        void matmul_tests::matmul_test2()
        {
            auto m1 = Matrix(3, 3);
            auto m2 = Matrix(5, 5);
            int success = 1;
            try
            {
                auto m3 = m1.matmul(m2);
                success = 0;
            }
            catch (const std::exception& e)
            {
                auto msg = e.what();
                EXPECT_EQ(msg[0], 'd'); // error message starts with "dimension..."
            }
            EXPECT_EQ(success, 1);
        }

        void matmul_tests::matvec_test()
        {
            auto m = Matrix(2, 2); // identity matrix
            m(0, 0) = 1.0;
            m(1, 1) = 1.0;
            m(0, 1) = 0.0;
            m(1, 0) = 0.0;
            auto v = vector1D({ M_PI, M_E });
            auto r = m.matvec(v); // r = v as m is the identity maxtrix
            for (size_t i = 0; i < v.size(); i++)
            {
                EXPECT_NEAR(r(i), v(i), margin);
            }
        }
    }
}
