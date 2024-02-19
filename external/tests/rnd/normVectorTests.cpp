#include <random>
#include <gtest/gtest.h>
#include "normVectorTests.h"
#include "../../src/rnd/george_marsaglia.h"
#include "../../src/rnd/mersenne_twister.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void normVectorTests::allNormVectorTests()
        {
            testGm();
            testMt1();
            testMt2();
            testMt3();
            checkMt19937();
        }

        void normVectorTests::checkMt19937()
        {   // test from: https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
            std::mt19937 gen32;
            std::mt19937_64 gen64;

            for (auto n{ 1 }; n != 10'000; gen32(), gen64(), ++n);

            auto next32 = gen32();
            auto next64 = gen64();
            ASSERT_EQ(next32, 4'123'659'995);
            ASSERT_EQ(next64, 9'981'545'732'273'789'042ULL);
        }

        void normVectorTests::testGm()
        {
            auto gm = george_marsaglia(2345, 3456);
            auto t = vector1D(2);
            gm.getNormalizedRandomVector(t);
            ASSERT_NEAR(t(0), -0.719288774, 1e-9);
            ASSERT_NEAR(t(1), -0.694711206, 1e-9);
        }

        void normVectorTests::testMt1()
        {
            auto mt = mersenne_twister(2345);
            auto r = mt.get_uniform();
            ASSERT_NEAR(r, 0.276389778, 1e-9);
        }

        void normVectorTests::testMt2()
        {
            auto mt = mersenne_twister(2345);
            auto r = mt.get_std_normal();
            ASSERT_NEAR(r, 0.593600187, 1e-9);
        }

        void normVectorTests::testMt3()
        {
            auto mt = mersenne_twister(2345);
            auto t = vector1D(2);
            mt.getNormalizedRandomVector(t);
            ASSERT_NEAR(t(0), 0.480461960, 1e-9);
            ASSERT_NEAR(t(1), 0.877015567, 1e-9);
        }
    }
}
