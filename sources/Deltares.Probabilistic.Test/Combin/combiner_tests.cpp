#include <fstream>
#include <stdio.h>
#include "gtest/gtest.h"
#include "combiner_tests.h"
#include "../../Deltares.Probabilistic/Combine/HohenbichlerCombiner.h"
#include "../../Deltares.Probabilistic/Combine/DirectionalSamplingCombiner.h"
#include "../../Deltares.Probabilistic/Combine/ImportanceSamplingCombiner.h"
#include "../../Deltares.Probabilistic/Combine/upscaling.h"

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
                margin = 1e-7;
                auto hh = std::make_unique<HohenbichlerCombiner>();
                auto ref = alphaBeta(1.9791111899137395, // pre-computed
                    { 0.70367242324781842, 0.43749837909101225, 0.47566443271219239, 0.29526875301302696 }); // pre-computed
                tester(hh.get(), ref);
            }

            void CombinerTest::DirectionalSamplingCombinerTest()
            {
                margin = 0.1;
                auto dsCombiner = std::make_unique<DirectionalSamplingCombiner>();
                dsCombiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(2.9873932909104584, // pre-computed
                    { 0.57465001696833640, 0.39885538364093981, 0.59196640545171397, 0.40033425503634662 }); // pre-computed
                tester(dsCombiner.get(), ref);
            }

            void CombinerTest::ImportanceSamplingCombinerTest()
            {
                margin = 0.1;
                auto importance_sampling_combiner = std::make_unique<ImportanceSamplingCombiner>();
                importance_sampling_combiner->randomGeneratorType = Numeric::MersenneTwister;
                auto ref = alphaBeta(3.0, { 0.0, 0.0, 0.0, 0.0}); // pre-computed
                tester(importance_sampling_combiner.get(), ref);
            }

            // test based on testcombineMultipleElementsSpatialCorrelated1
            void CombinerTest::tester(Combiner* comb, const alphaBeta& ref)
            {
                const int nElements = 20; // Number of elements
                const size_t nStochasts = 4;

                std::vector< std::shared_ptr<Stochast>> stochasts;
                for (size_t i = 0; i < nStochasts; i++)
                {
                    stochasts.push_back(std::make_shared<Stochast>());
                }

                auto CrossSection = alphaBeta(3.0, { 0.6, sqrt(0.5 - 0.36), 0.6, sqrt(0.5 - 0.36) });
                auto rhoXK = vector1D({ 0.5, 0.5, 0.2, 0.2 });
                auto dXK = vector1D({ 500.0, 300.0, 500.0, 300.0 });
                double sectionLength = 100.0;


                auto up = upscaling();
                auto section = up.upscaleLength(CrossSection, rhoXK, dXK, sectionLength, -999.0);
                EXPECT_EQ(section.second, 0);
                auto Elements = std::vector<std::shared_ptr<DesignPoint>>();
                for (size_t i = 0; i < nElements; i++)
                {
                    auto dp = std::make_shared<DesignPoint>();
                    dp->Beta = section.first.getBeta();
                    for (size_t j = 0; j < nStochasts; j++)
                    {
                        auto alpha = std::make_shared<StochastPointAlpha>();
                        alpha->Alpha = section.first.getAlphaI(j);
                        alpha->Stochast = stochasts[j];
                        dp->Alphas.push_back(alpha);
                    }
                    Elements.push_back(dp);
                }

                auto rho = std::make_shared<SelfCorrelationMatrix>();
                for (size_t i = 0; i < nStochasts; i++)
                {
                    rho->setSelfCorrelation(Elements[0]->Alphas[i]->Stochast, rhoXK(i));
                }

                auto cmbDp = comb->combineDesignPoints(combineAndOr::combOr, Elements, rho, nullptr);

                EXPECT_NEAR(cmbDp->Beta, ref.getBeta(), margin);
                for (size_t i = 0; i < nStochasts; i++)
                {
                    EXPECT_NEAR(cmbDp->Alphas[i]->Alpha, ref.getAlphaI(i), margin);
                }
            }

        }
    }
}
