#include <gtest/gtest.h>
#include "testStartPointCalculator.h"
#include "../../Deltares.Probabilistic/Reliability/StartPointCalculator.h"

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
                auto modelRunner = BuildProject();
                auto calculator = StartPointCalculator();

                modelRunner->updateStochastSettings(calculator.Settings->StochastSet);
                calculator.Settings->StartMethod = StartMethodType::One;

                auto r = calculator.getStartPoint(modelRunner);

                ASSERT_EQ(r->Values.size(), 2);
                EXPECT_EQ(r->Values[0], 1.0);
                EXPECT_EQ(r->Values[1], 1.0);
            }

            std::shared_ptr<ModelRunner> testStartPointCalculator::BuildProject()
            {
                std::shared_ptr<ZModel> z(new ZModel(ZModel([this](std::shared_ptr<Sample> v) { return zfunc(v); })));
                auto stochast = std::vector<std::shared_ptr<Stochast>>();
                auto dist = DistributionType::Normal;
                std::vector<double> params{ 0.0, 1.0 };
                std::shared_ptr<Stochast> s(new Stochast(dist, params));
                stochast.push_back(s);
                stochast.push_back(s);
                std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
                std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
                uConverter->initializeForRun();
                std::shared_ptr<ModelRunner> m(new ModelRunner(z, uConverter));
                return m;
            }

            void testStartPointCalculator::zfunc(std::shared_ptr<Deltares::Models::Sample> sample)
            {
                sample->Z = sample->Values[1] - sample->Values[0];
            }
        }
    }
}
