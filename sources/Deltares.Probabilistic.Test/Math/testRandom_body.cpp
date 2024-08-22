#include "testRandom.h"
#include "../../Deltares.Probabilistic/Math/Randomizers/GeorgeMarsagliaRandomValueGenerator.h"
#include "../../Deltares.Probabilistic/Math/Randomizers/MersenneTwisterRandomValueGenerator.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            using namespace Deltares::Numeric;

            void testRandom::allRandomTests()
            {
                georgeMarsagliaTest1();
                georgeMarsagliaTest2();
                mersenneTwisterTest1();
                initializationTest();
            }

            void testRandom::georgeMarsagliaTest1()
            {
                auto gm = GeorgeMarsagliaRandomValueGenerator();
                constexpr int ij = 10000;
                constexpr int kl = 10000;
                gm.initialize(true, ij, kl);

                auto result = gm.next();
                ASSERT_NEAR(result, 0.755450129508972, margin);
            }

            void testRandom::georgeMarsagliaTest2()
            {
                auto gm = GeorgeMarsagliaRandomValueGenerator();
                constexpr int ij = 177;
                constexpr int kl = 177;
                constexpr int n = 100000;
                gm.initialize(true, ij, kl);

                double result = 0.0;
                for (int i = 0; i < n; i++)
                {
                    result = gm.next();
                }
                ASSERT_NEAR(result, 0.7785768508911, margin);
            }

            void testRandom::mersenneTwisterTest1()
            {
                auto mt = MersenneTwisterRandomValueGenerator();
                mt.initialize(true, 1, 2);

                double sum = 0.0;
                for (size_t i = 0; i < 1000; i++)
                {
                    sum += mt.next();
                }
                ASSERT_NEAR(sum, 510.36972065202127, margin);
            }

            void testRandom::initializationTest()
            {
                auto gm = GeorgeMarsagliaRandomValueGenerator();
                gm.initialize(false, 0, 0);
                auto mt = MersenneTwisterRandomValueGenerator();
                mt.initialize(false, 0, 0);

                double val1 = gm.next();
                double val2 = mt.next();
                ASSERT_TRUE(val1 >= 0.0 && val1 <= 1.0);
                ASSERT_TRUE(val2 >= 0.0 && val2 <= 1.0);
            }

        }
    }
}

