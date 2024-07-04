#include "testRandom.h"
#include "../../Deltares.Probabilistic/Math/Randomizers/GeorgeMarsagliaRandomValueGenerator.h"

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
        }
    }
}

