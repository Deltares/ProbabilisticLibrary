// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include <fstream>
#include "gtest/gtest.h"
#include "combiner_tests.h"
#include "../../Deltares.Probabilistic/Math/NumericSupport.h"
#include "../../Deltares.Probabilistic/Combine/HohenbichlerFormCombiner.h"
#include "../../Deltares.Probabilistic/Combine/HohenbichlerNumIntCombiner.h"
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
                Hohenbichler2CombinerTest();
                HohenbichlerCombiner1StochTest();
                Hohenbichler2Combiner1StochTest();
                DirectionalSamplingCombinerTest();
                ImportanceSamplingCombinerTest();
                ImportanceSamplingCombinerAndTest();
                ImportanceSamplingCombinerInvertedTest();
            }

            void CombinerTest::HohenbichlerCombinerTest() const
            {
                auto hh = std::make_unique<HohenbichlerFormCombiner>();
                auto ref = alphaBeta(2.78586434, { 0.6145053, 0.378711, 0.5892747, 0.3629275 }); // pre-computed
                tester(hh.get(), 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::Hohenbichler2CombinerTest() const
            {
                auto hh = std::make_unique<HohenbichlerNumIntCombiner>();
                auto ref = alphaBeta(2.786202, { 0.634747,  0.391671, 0.566766,  0.3499527 }); // pre-computed
                tester(hh.get(), 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::HohenbichlerCombiner1StochTest() const
            {
                auto hh = std::make_unique<HohenbichlerFormCombiner>();
                auto ref = alphaBeta(3.0, { 1.0 }); // pre-computed
                tester1stoch(hh.get(), 1.0, 3.0, ref, combineAndOr::combOr);

                ref = alphaBeta(3.0, { 1.0 }); // pre-computed
                tester1stoch(hh.get(), 1.0, 3.0, ref, combineAndOr::combAnd);

                ref = alphaBeta(2.782175, { 1.0 }); // pre-computed
                tester1stoch(hh.get(), -1.0, 3.0, ref, combineAndOr::combOr);
                ASSERT_EQ(2, hh->nonConvergedForm);
            }

            void CombinerTest::Hohenbichler2Combiner1StochTest() const
            {
                auto hh = std::make_unique<HohenbichlerNumIntCombiner>();
                auto ref = alphaBeta(3.0, { 1.0 }); // pre-computed
                tester1stoch(hh.get(), 1.0, 3.0, ref, combineAndOr::combOr);

                ref = alphaBeta(3.0, { 1.0 }); // pre-computed
                tester1stoch(hh.get(), 1.0, 3.0, ref, combineAndOr::combAnd);

                ref = alphaBeta(-40.0, { 1.0 }); // pre-computed
                tester1stoch(hh.get(), -1.0, 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::DirectionalSamplingCombinerTest() const
            {
                auto dsCombiner = std::make_unique<DirectionalSamplingCombiner>();
                dsCombiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(2.82397, { 0.656456, 0.399822, 0.549539, 0.3274373 }); // pre-computed
                tester(dsCombiner.get(), 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::ImportanceSamplingCombinerTest() const
            {
                auto importance_sampling_combiner = std::make_unique<ImportanceSamplingCombiner>();
                importance_sampling_combiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(2.78826, { 0.64757, 0.39934, 0.64757, 0.39934 }); // pre-computed
                tester(importance_sampling_combiner.get(), 3.0, ref, combineAndOr::combOr);
            }

            void CombinerTest::ImportanceSamplingCombinerAndTest() const
            {
                auto importance_sampling_combiner = std::make_unique<ImportanceSamplingCombiner>();
                importance_sampling_combiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(4.13320, { 0.66929, 0.51149, 0.42482, 0.33159 }); // pre-computed
                tester(importance_sampling_combiner.get(), 3.0, ref, combineAndOr::combAnd);
            }

            void CombinerTest::ImportanceSamplingCombinerInvertedTest() const
            {
                auto importance_sampling_combiner = std::make_unique<ImportanceSamplingCombiner>();
                importance_sampling_combiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(0.60732, { 0.632059, 0.39174, 0.571165, 0.34758 }); // pre-computed
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

            void CombinerTest::tester1stoch(Combiner* comb, const double rho, const double beta, const alphaBeta& ref, const combineAndOr AndOr) const
            {
                constexpr int nElements = 2; // Number of elements
                constexpr size_t nStochasts = 1;

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
                        auto alpha = std::make_shared<StochastPointAlpha>();
                        alpha->Alpha = 1.0;
                        alpha->Stochast = stochasts[j];
                        alpha->U = -dp->Beta * alpha->Alpha;
                        dp->Alphas.push_back(alpha);
                    }
                    Elements.push_back(dp);
                }

                auto selfCorrelation = std::make_shared<SelfCorrelationMatrix>();
                for (size_t i = 0; i < nStochasts; i++)
                {
                    selfCorrelation->setSelfCorrelation(Elements[0]->Alphas[i]->Stochast, rho);
                }

                auto cmbDp = comb->combineDesignPoints(AndOr, Elements, selfCorrelation, nullptr);

                EXPECT_NEAR(cmbDp->Beta, ref.getBeta(), margin);
                for (size_t i = 0; i < nStochasts; i++)
                {
                    EXPECT_NEAR(cmbDp->Alphas[i]->Alpha, ref.getAlphaI(i), margin);
                }
            }

        }
    }
}
