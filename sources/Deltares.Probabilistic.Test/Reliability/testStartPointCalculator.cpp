#include <gtest/gtest.h>
#include "testStartPointCalculator.h"
#include "../../Deltares.Probabilistic/Reliability/StartPointCalculator.h"
#include "../projectBuilder.h"

using namespace Deltares::Reliability;
using namespace Deltares::Statistics;
using namespace Deltares::Models;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void testStartPointCalculator::allStartPointTests()
            {
                testMethodOne();
            }

            void testStartPointCalculator::testMethodOne()
            {
                auto modelRunner = projectBuilder().BuildProject();
                auto calculator = StartPointCalculator();

                modelRunner->updateStochastSettings(calculator.Settings->StochastSet);
                calculator.Settings->StartMethod = StartMethodType::One;

                auto r = calculator.getStartPoint(modelRunner);

                ASSERT_EQ(r->Values.size(), 2);
                EXPECT_EQ(r->Values[0], 1.0);
                EXPECT_EQ(r->Values[1], 1.0);
            }

        }
    }
}
