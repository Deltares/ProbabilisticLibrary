#include "probNumber_tests.h"
#include <gtest/gtest.h>
#include "../../src/probFuncs/probNumber.h"
#include "../../src/probFuncs/conversions.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void probNumber_tests::all_probNumber_tests()
        {
            probNumber_test1();
            probNumber_test2();
            probNumber_test3();
            probNumber_test4();
            probNumber_test5();
        }

        void probNumber_tests::probNumber_test1()
        {
            auto u = probNumber(0.0);
            ASSERT_NEAR(u.getp(), 0.5, 1e-12);
            ASSERT_NEAR(u.getq(), 0.5, 1e-12);
        }

        void probNumber_tests::probNumber_test2()
        {
            auto p = probNumber(0.5, 'p');
            ASSERT_EQ(p.getp(), 0.5);
            ASSERT_EQ(p.getq(), 0.5);
            ASSERT_NEAR(p.getu(), 0.0, 1e-12);
        }

        void probNumber_tests::probNumber_test3()
        {
            auto q = probNumber(0.5, 'q');
            ASSERT_EQ(q.getp(), 0.5);
            ASSERT_EQ(q.getq(), 0.5);
            ASSERT_NEAR(q.getu(), 0.0, 1e-12);
        }

        void probNumber_tests::probNumber_test4()
        {
            for (int i = -40; i < 41; i++)
            {
                auto beta = probNumber(i);
                ASSERT_EQ(beta.getu(), (double)i);
                ASSERT_EQ(beta.getp(), conversions::PfromBeta(i));
                ASSERT_EQ(beta.getq(), conversions::QfromBeta(i));
            }
        }

        void probNumber_tests::probNumber_test5()
        {
            for (int i = -37; i < 0; i++)
            {
                const double uu = (double)i;
                const double p = conversions::PfromBeta(uu);
                auto x = probNumber(p, 'p');
                const double u = x.getu();
                ASSERT_NEAR(u, uu, 1e-8);
            }
        }
    }
}
