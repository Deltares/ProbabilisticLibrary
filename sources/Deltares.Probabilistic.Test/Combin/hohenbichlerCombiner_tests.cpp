#include <fstream>
#include <stdio.h>
#include "gtest/gtest.h"
#include "hohenbichlerCombiner_tests.h"
#include "../../Deltares.Probabilistic/Combine/HohenbichlerCombiner.h"
#include "../../Deltares.Probabilistic/Combine/upscaling.h"

using namespace Deltares::Reliability;
using namespace Deltares::Statistics;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void HohenbichlerCombinerTest::allHohenbichlerCombinerTests()
            {
                test1();
            }

            // test based on testcombineMultipleElementsSpatialCorrelated1
            void HohenbichlerCombinerTest::test1()
            {
                const int nElements = 20; // Number of elements
                const size_t nStochasts = 4;

                std::vector< std::shared_ptr<Stochast>> stochasts;
                for (size_t i = 0; i < nStochasts; i++)
                {
                    stochasts.push_back(std::make_shared<Stochast>());
                }

                auto CrossSection = alphaBeta(5.0, { 0.6, sqrt(0.5 - 0.36), 0.6, sqrt(0.5 - 0.36) });
                auto rhoXK = vector1D({ 0.5, 0.5, 0.2, 0.2 });
                auto dXK = vector1D({ 500.0, 300.0, 500.0, 300.0 });
                double sectionLength = 100.0;

                auto ref = alphaBeta(4.38787743765301, // pre-computed
                    { 0.635285167139092, 0.393519326675495, 0.565059833788674, 0.349660807332817 }); // pre-computed

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

                auto hh = HohenbichlerCombiner();


                auto rho = std::make_shared<SelfCorrelationMatrix>();
                for (size_t i = 0; i < nStochasts; i++)
                {
                    rho->setSelfCorrelation(Elements[0]->Alphas[i]->Stochast, rhoXK(i));
                }

                auto cmbDp = hh.combineDesignPoints(combineAndOr::combOr, Elements, rho);

                EXPECT_NEAR(cmbDp->Beta, ref.getBeta(), margin);
                for (size_t i = 0; i < nStochasts; i++)
                {
                    EXPECT_NEAR(cmbDp->Alphas[i]->Alpha, ref.getAlphaI(i), margin);
                }
            }

        }
    }
}
