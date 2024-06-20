#include <fstream>
#include "gtest/gtest.h"
#include "combiner_tests.h"
#include "../../Deltares.Probabilistic/Math/NumericSupport.h"
#include "../../Deltares.Probabilistic/Combine/HohenbichlerCombiner.h"
#include "../../Deltares.Probabilistic/Combine/DirectionalSamplingCombiner.h"
#include "../../Deltares.Probabilistic/Combine/ImportanceSamplingCombiner.h"

using namespace Deltares::Reliability;
using namespace Deltares::Statistics;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void CombinerTest::allCombinerTests() const
            {
                HohenbichlerCombinerTest();
                DirectionalSamplingCombinerTest();
                ImportanceSamplingCombinerTest();
                ImportanceSamplingCombinerAndTest();
                ImportanceSamplingCombinerInvertedTest();
            }

            void CombinerTest::HohenbichlerCombinerTest() const
            {
                auto hh = std::make_unique<HohenbichlerCombiner>();
                auto ref = alphaBeta(2.78586434, { 0.6145053, 0.378711, 0.5892747, 0.3629275 }); // pre-computed
                tester(hh.get(), 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::DirectionalSamplingCombinerTest() const
            {
                auto dsCombiner = std::make_unique<DirectionalSamplingCombiner>();
                dsCombiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(2.96662, { 0.60645, 0.34410, 0.60154, 0.38984 }); // pre-computed
                tester(dsCombiner.get(), 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::ImportanceSamplingCombinerTest() const
            {
                auto importance_sampling_combiner = std::make_unique<ImportanceSamplingCombiner>();
                importance_sampling_combiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(3.0, { 0.60187, 0.37115, 0.60187, 0.37115 }); // pre-computed
                tester(importance_sampling_combiner.get(), 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::ImportanceSamplingCombinerAndTest() const
            {
                auto importance_sampling_combiner = std::make_unique<ImportanceSamplingCombiner>();
                importance_sampling_combiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(2.96369, { 0.57367, 0.35851, 0.60798, 0.41561 }); // pre-computed
                tester(importance_sampling_combiner.get(), 3.0, ref, combineAndOr::combAnd);
            }

            void CombinerTest::ImportanceSamplingCombinerInvertedTest() const
            {
                auto importance_sampling_combiner = std::make_unique<ImportanceSamplingCombiner>();
                importance_sampling_combiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(1.00026, { 0.60469, 0.35473, 0.606108, 0.37570 }); // pre-computed
                tester(importance_sampling_combiner.get(), 1.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::tester(Combiner* comb, const double beta, const alphaBeta& ref, const combineAndOr AndOr ) const
            {
                constexpr int nElements = 2; // Number of elements
                constexpr size_t nStochasts = 4;

                const auto rhoXK = std::vector<double>({ 0.5, 0.5, 0.2, 0.2 });
                auto alphaInput = std::vector<double>{ 0.6, 0.37, 0.6, 0.37 };
                const auto length = Numeric::NumericSupport::GetLength(alphaInput);
                for (auto &x : alphaInput) { x /= length; }

                std::vector< std::shared_ptr<Stochast>> stochasts;
                for (size_t i = 0; i <= nStochasts; i++)
                {
                    auto s = std::make_shared<Stochast>();
                    stochasts.push_back(s);
                }

                auto Elements = std::vector<std::shared_ptr<DesignPoint>>();
                for (size_t i = 0; i < nElements; i++)
                {
                    auto dp = std::make_shared<DesignPoint>();
                    dp->Beta = beta;
                    for (size_t j = 0; j < nStochasts; j++)
                    {
                        // swap order stochasts for 2nd element:
                        auto jj = (i == 0 ? j : nStochasts - 1 - j);
                        auto alpha = std::make_shared<StochastPointAlpha>();
                        alpha->Alpha = alphaInput[jj];
                        alpha->Stochast = stochasts[jj];
                        alpha->U = -dp->Beta * alpha->Alpha;
                        dp->Alphas.push_back(alpha);
                    }
                    if (i == 1)
                    {
                        // add stochasts with alpha = 0 to have different sized sets of design points
                        auto alpha = std::make_shared<StochastPointAlpha>();
                        alpha->Alpha = 0.0;
                        alpha->Stochast = stochasts[nStochasts];
                        alpha->U = 0.0;
                        dp->Alphas.push_back(alpha);
                    }
                    Elements.push_back(dp);
                }

                auto rho = std::make_shared<SelfCorrelationMatrix>();
                for (size_t i = 0; i < nStochasts; i++)
                {
                    rho->setSelfCorrelation(Elements[0]->Alphas[i]->Stochast, rhoXK[i]);
                }

                auto cmbDp = comb->combineDesignPoints(AndOr, Elements, rho, nullptr);

                EXPECT_NEAR(cmbDp->Beta, ref.getBeta(), margin);
                for (size_t i = 0; i < nStochasts; i++)
                {
                    EXPECT_NEAR(cmbDp->Alphas[i]->Alpha, ref.getAlphaI(i), margin);
                }
            }

        }
    }
}
