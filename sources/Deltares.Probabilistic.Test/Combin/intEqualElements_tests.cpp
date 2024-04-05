#include <gtest/gtest.h>
#include <math.h>
#include "intEqualElements_tests.h"
#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"

using namespace Deltares::Statistics;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void intEqualElements_tests::allIntegrateEqualElementsTests()
            {
                integrateEqualElementsTests1();
                integrateEqualElementsTests2();
                integrateEqualElementsTests3();
                integrateEqualElementsTests4();
            }

            // Testing the upscaling of probabilities in time.
            // Here the part governed by integrateEqualElements().
            // Beta=5.0, rhoT= 0.0, and nrElements= 10
            // The resulting betaT can analytically be computed and compared to the numerically computed result.
            void intEqualElements_tests::integrateEqualElementsTests1()
            {
                const double beta = 5.0;
                const double rhoT = 0.0;
                const int nrElements = 10;
                const double betaT = intEqElm.integrateEqualElements(beta, rhoT, nrElements);
                //  Compute analytically the expected betaT:
                const double pF = 1.0 - pow(StandardNormal::getPFromU(beta), nrElements);
                const double expectedBetaT = StandardNormal::getUFromQ(pF);

                const double myMargin = 1.0e-5;
                ASSERT_NEAR(betaT, expectedBetaT, myMargin);
            }

            // Testing the upscaling of probabilities in time.
            // Here the part governed by integrateEqualElements().
            // Beta=5.0, rhoT= 0.5, and nrElements= 10
            // The resulting betaT is computed and compared to its expected value
            void intEqualElements_tests::integrateEqualElementsTests2()
            {
                const double beta = 5.0;
                const double rhoT = 0.5;
                const int nrElements = 10;
                const double betaT = intEqElm.integrateEqualElements(beta, rhoT, nrElements);
                //   The expected betaT is precomputed as:
                const double expectedBetaT = 4.53853038583672;
                const double myMargin = 1.0e-6;
                ASSERT_NEAR(betaT, expectedBetaT, myMargin);
            }

            // Testing the upscaling of probabilities in time.
            // Here the part governed by integrateEqualElements().
            // Test with a high rhoT.
            // Beta=5.0, rhoT= 0.999999, and nrElements= 10
            // The resulting betaT is computed and compared to its expected value
            void intEqualElements_tests::integrateEqualElementsTests3()
            {
                const double beta = 5.0;
                const double rhoT = 0.999999;
                const double nrElements = 10;
                const double betaT = intEqElm.integrateEqualElements(beta, rhoT, nrElements);
                //  The expected betaT must be very close to beta and is precomputed as:
                const double expectedBetaT = 4.99846288959719;
                const double myMargin = 1.0e-6;
                ASSERT_NEAR(betaT, expectedBetaT, myMargin);
            }

            // Testing the upscaling of probabilities in time.
            // Here the part governed by integrateEqualElements().
            // Test with a high rhoT and large number of elements.
            // Beta=5.0, rhoT= 0.999999, and nrElements= 10000
            // The resulting betaT is computed and compared to its expected value
            void intEqualElements_tests::integrateEqualElementsTests4()
            {
                const double beta = 5.0;
                const double rhoT = 0.999999;
                const double nrElements = 1000;
                const double betaT = intEqElm.integrateEqualElements(beta, rhoT, nrElements);
                //  For the present input parameters the expected betaT must be very close to beta and is precomputed as:
                const double expectedBetaT = 4.99676192410764;
                const double myMargin = 1.0e-6;
                ASSERT_NEAR(betaT, expectedBetaT, myMargin);
            }

        }

    }
}
