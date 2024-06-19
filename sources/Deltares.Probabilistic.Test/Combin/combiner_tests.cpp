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
            void CombinerTest::allCombinerTests()
            {
                HohenbichlerCombinerTest();
                DirectionalSamplingCombinerTest();
                ImportanceSamplingCombinerTest();
            }

            void CombinerTest::HohenbichlerCombinerTest()
            {
                margin = 1e-4;
                auto hh = std::make_unique<HohenbichlerCombiner>();
                auto ref = alphaBeta(2.78586434, { 0.6145053, 0.378711, 0.5892747, 0.3629275 }); // pre-computed
                tester(hh.get(), 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::DirectionalSamplingCombinerTest()
            {
                margin = 0.1;
                auto dsCombiner = std::make_unique<DirectionalSamplingCombiner>();
                dsCombiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(3.0139519, { 0.56572, 0.41313, 0.58968, 0.40193 }); // pre-computed
                tester(dsCombiner.get(), 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::ImportanceSamplingCombinerTest()
            {
                margin = 0.01;
                auto importance_sampling_combiner = std::make_unique<ImportanceSamplingCombiner>();
                importance_sampling_combiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(3.0, { 0.6, 0.374, 0.6, 0.374 }); // pre-computed
                tester(importance_sampling_combiner.get(), 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::ImportanceSamplingCombinerAndTest()
            {
                margin = 0.01;
                auto importance_sampling_combiner = std::make_unique<ImportanceSamplingCombiner>();
                importance_sampling_combiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(2.96511, { 0.584, 0.36424, 0.602, 0.4038 }); // pre-computed
                tester(importance_sampling_combiner.get(), 3.0, ref, combineAndOr::combAnd);
            }

            void CombinerTest::ImportanceSamplingCombinerInvertedTest()
            {
                margin = 0.01;
                auto importance_sampling_combiner = std::make_unique<ImportanceSamplingCombiner>();
                importance_sampling_combiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(0.99917, { 0.603, 0.373, 0.5898, 0.3861 }); // pre-computed
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
