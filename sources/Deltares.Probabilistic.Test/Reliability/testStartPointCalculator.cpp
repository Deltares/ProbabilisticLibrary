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
                testMethodRaySearch();
                testMethodSphereSearch();
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

            void testStartPointCalculator::testMethodRaySearch()
            {
                auto modelRunner = projectBuilder().BuildProject();
                auto calculator = StartPointCalculator();

                modelRunner->updateStochastSettings(calculator.Settings->StochastSet);
                calculator.Settings->StartMethod = StartMethodType::RaySearch;
                calculator.Settings->MaximumLengthStartPoint = 20.0;
                calculator.Settings->dsdu = 3.0;

                auto r = calculator.getStartPoint(modelRunner);

                ASSERT_EQ(r->Values.size(), 2);
                EXPECT_NEAR(r->Values[0], 12.0, margin);
                EXPECT_NEAR(r->Values[1], 12.0, margin);
            }

            void testStartPointCalculator::testMethodSphereSearch()
            {
                auto modelRunner = projectBuilder().BuildProject();
                auto calculator = StartPointCalculator();

                modelRunner->updateStochastSettings(calculator.Settings->StochastSet);
                calculator.Settings->StartMethod = StartMethodType::SphereSearch;
                calculator.Settings->MaximumLengthStartPoint = 20.0;
                calculator.Settings->dsdu = 3.0;

                auto r = calculator.getStartPoint(modelRunner);

                ASSERT_EQ(r->Values.size(), 2);
                EXPECT_NEAR(r->Values[0], 2.4, margin);
                EXPECT_NEAR(r->Values[1], 0.0, margin);
            }

        }
    }
}
