#include "pch.h"
#include <math.h>
#include <gtest/gtest.h>
#include "upscale_tests.h"
#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"

using namespace Deltares::Statistics;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void upscale_tests::runAll()
            {
                upscaleInTimeTests1();
                upscaleInTimeTests2();
                upscaleInTimeTests3();
                upscaleInTimeTests4();
                upscaleInTimeTests5();
                upscaleInTimeTests6();
                upscaleInTimeTests7();
                upscaleInTimeTests8();
                EquivalentAlphaTesting1();
                EquivalentAlphaTesting2();
                EquivalentAlphaTesting3();
                EquivalentAlphaTesting4();
                EquivalentAlphaTesting5();
                EquivalentAlphaTesting6();
                upscaleToLargestBlockTests1();
                upscaleToLargestBlockTests2();
                upscaleToLargestBlockTests3();
            }

            // Testing the upscaling of probabilities in time.
            // Test with 10 fully correlated elements.
            // Beta=3.5, and the design point is equal to (1 1 1 1 1)/Sqrt(5)
            // The resulting beta and alfa's should be equal to the values for the single element
            void upscale_tests::upscaleInTimeTests1()
            {
                const size_t nstoch = 5;
                const int nrElements = 10;
                auto element = alphaBeta(3.5, vector1D(nstoch));     //equal alpha values
                element.assign(1.0);
                element.normalize();
                auto inrhot = vector1D(nstoch);
                inrhot.assign(1.0); // all rho values set to one

                auto elementORG = element;  // Copy of the original beta and alpha values

                up.upscaleInTime(nrElements, element, inrhot);

                ut.checkAlphaBeta(element, elementORG, 0.001);
            }

            // Testing the upscaling of probabilities in time.
            // Test with 10 uncorrelated elements.
            // Beta=3.5, and the design point is equal to (1 0 0 0 0)
            // The resulting beta is verified and alfa's should be equal to the values for the single element
            void upscale_tests::upscaleInTimeTests2()
            {
                const size_t nstoch = 5;
                const int nrElements = 10;
                auto element = alphaBeta(3.5, { 1.0, 0.0, 0.0, 0.0, 0.0 });
                auto inrhot = vector1D(nstoch); // defaults to 0.0

                const auto p = element.getP();
                auto beta = StandardNormal::getUFromQ(1.0 - pow(p, nrElements));
                auto ref = alphaBeta(beta, element.getAlpha());

                up.upscaleInTime(nrElements, element, inrhot);

                ut.checkAlphaBeta(element, ref, 0.001);
            }

            // Testing the upscaling of probabilities in time.
            // Test with 10 uncorrelated elements.
            // Beta=3.5, and the design point is equal to (0 0.6 0 0.8 0)
            // The resulting beta is verified and alfa's should be equal to the values for the single element
            void upscale_tests::upscaleInTimeTests3()
            {
                const size_t nstoch = 5;
                const int nrElements = 10;
                auto element = alphaBeta(3.5, { 0.0, 0.6, 0.0, 0.8, 0.0 });
                const auto inrhot = vector1D(nstoch);

                const auto p = element.getP();
                auto beta = StandardNormal::getUFromQ(1.0 - pow(p, nrElements));
                auto ref = alphaBeta(beta, element.getAlpha());

                up.upscaleInTime(nrElements, element, inrhot);

                ut.checkAlphaBeta(element, ref, 0.001);
            }

            // Testing the upscaling of probabilities in time.
            // Test with 10 uncorrelated elements.
            // Beta=3.5, and the design point is equal to (-1 0 0 0 0)
            // The resulting beta is verified and alfa's should be equal to the values for the single element
            void upscale_tests::upscaleInTimeTests4()
            {
                const size_t nstoch = 5;
                const int nrElements = 10;
                auto element = alphaBeta(3.5, { -1.0, 0.0, 0.0, 0.0, 0.0 });
                auto inrhot = vector1D(nstoch);

                const auto p = element.getP();
                auto beta = StandardNormal::getUFromQ(1.0 - pow(p, nrElements));
                auto ref = alphaBeta(beta, element.getAlpha());

                up.upscaleInTime(nrElements, element, inrhot);

                ut.checkAlphaBeta(element, ref, 0.001);
            }

            // Testing the upscaling of probabilities in time.
            // Test with 10 uncorrelated elements.
            // Beta=3.5, and the design point is equal to (0 0.6 0 -0.8 0)
            // The resulting beta is verified and alfa's should be equal to the values for the single element
            void upscale_tests::upscaleInTimeTests5()
            {
                const size_t nstoch = 5;
                const int nrElements = 10;
                auto element = alphaBeta(3.5, { 0.0, 0.6, 0.0, -0.8, 0.0 });
                auto inrhot = vector1D(nstoch);

                const auto p = element.getP();
                auto beta = StandardNormal::getUFromQ(1.0 - pow(p, nrElements));
                auto ref = alphaBeta(beta, element.getAlpha());

                up.upscaleInTime(nrElements, element, inrhot);

                ut.checkAlphaBeta(element, ref, 0.001);
            }

            // Testing the upscaling of probabilities in time.
            // Test with 10 almost fully correlated elements.
            // Beta= 3.5, and the design point is equal to (0, 0.6, 0, -0.8, 0)
            // The resulting beta is verified and alfa's should be equal to the values for the single element
            void upscale_tests::upscaleInTimeTests6()
            {
                const size_t nstoch = 5;
                const int nrElements = 10;
                auto element = alphaBeta(3.5, { 0.0, 0.6, 0.0, -0.8, 0.0 });
                auto inRhoT = vector1D(nstoch);
                inRhoT.assign(0.999999);

                auto ref = alphaBeta(3.49846239808633, element.getAlpha());

                up.upscaleInTime(nrElements, element, inRhoT);

                ut.checkAlphaBeta(element, ref, 1.0e-6);
            }

            // case from HRING-1349
            // two almost identical situations before upscaling in time.
            // however, the resulting alpha(12) differs largely
            void upscale_tests::upscaleInTimeTests7()
            {
                const double margin = 1.0e-6; // acceptable margin for difference between beta original and beta computed
                const size_t nStoch = 12;      // Number of stochastic variables
                const double nrElements = 365.25; // Number of time elements (e.g. tidal periods)

                // case 1:
                auto element = alphaBeta(11.57868694964,
                    { -3.161166262316398E-6, 0.151673576904618, -8.746544990273167E-2,
                     -8.746544990273167E-2, -0.156371455735438,    -0.912570913068033,
                      0.250863571511060,     3.568922013287609E-4,  0.203773500205695,
                      3.810363930428925E-4,  1.919024503886323E-4, -7.296398126214022E-4 });
                auto inRhoT = vector1D(nStoch);
                inRhoT.assign(1.0);
                inRhoT(nStoch - 1) = 0.0;

                up.upscaleInTime(nrElements, element, inRhoT);

                ASSERT_NEAR(element.getBeta(), 11.5765466697374, margin);
                ASSERT_NEAR(element.getAlphaI(nStoch - 1), -0.311985E-3, margin);

                // case 2:
                // relative differences with case 1 approx. 1E-4
                element = alphaBeta(11.5805537878769,
                    { -3.161065009975362E-6,  0.151647352510170,    -8.745032983732001E-2,
                     -8.745032983732001E-2, -0.156344425365967,    -0.912591229826987,
                      0.250861020035668,     3.568801510567352E-4,  0.203738889137470,
                      3.809716860474264E-4,  1.918698629447029E-4, -7.296245519395209E-4 });

                up.upscaleInTime(nrElements, element, inRhoT);

                ASSERT_NEAR(element.getBeta(), 11.5783936419198, margin);
                ASSERT_NEAR(element.getAlphaI(nStoch - 1), -0.27259E-3, margin);
            }

            // test upscaling with nrElements = 1.0
            // this should give the same alpha and beta as the input values.
            // however, the alpha values differ ~ 1e-10
            void upscale_tests::upscaleInTimeTests8()
            {
                const double marginBeta = 1.0e-13; // acceptable margin for difference between beta original and beta computed
                const double marginAlpha = 1.0e-10; // acceptable margin for difference between alpha original and alpha computed
                const size_t nStoch = 2;       // Number of stochastic variables
                const double nrElements = 1.0; // Number of time elements (e.g. tidal periods)

                auto element = alphaBeta(4.8, { 1.0, 0.001 });
                element.normalize();
                auto inRhoT = vector1D(nStoch);
                inRhoT.assign(1.0);
                inRhoT(nStoch - 1) = 0.0;
                auto alphaRef = element.getAlpha();

                up.upscaleInTime(nrElements, element, inRhoT);

                ASSERT_NEAR(element.getBeta(), 4.8, marginBeta);
                ASSERT_NEAR(element.getAlphaI(0), alphaRef(0), marginAlpha);
                ASSERT_NEAR(element.getAlphaI(1), alphaRef(1), marginAlpha);
            }

            // Testing of the consistency of the equivalent alpha according to the methods
            // upscaleInTime and combineTwoElementsPartialCorrelation
            // Test with 2 partially correlated elements.
            // Beta is 3.5, the design point is equal to (0.6 0.8), and Rho is equal to (0.5 0.5)
            // The resulting beta is verified and resulting alfa's should be (approximately) equal
            void upscale_tests::EquivalentAlphaTesting1()
            {
                //  Initialisations:
                const int nrElements = 2;

                auto elm = alphaBeta(3.5, { 0.6, 0.8 });
                auto inRhoT = vector1D({ 0.5, 0.5 });

                auto elmORG = elm;   // copy of original beta

                up.upscaleInTime(nrElements, elm, inRhoT);
                auto a1 = vector1D(); auto a2 = vector1D();
                auto elm3 = cmb.combineTwoElementsPartialCorrelation(elmORG, elmORG, inRhoT, combineAndOr::combOr, a1, a2);

                ut.checkAlphaBeta(elm, elm3.ab, 1.0e-3);
                EXPECT_EQ(elm3.n, 0);
            }

            // Testing of the consistency of the equivalent alpha according to the methodes upscaleInTime and combineTwoElementsPartialCorrelation
            // Test with 2 partially correlated elements.
            // Beta is 3.5, the design point is equal to (0.6 0.8), and Rho is equal to (1.0 0.5)
            // The resulting beta is verified and resulting alfa's should be (approximately) equal
            void upscale_tests::EquivalentAlphaTesting2()
            {
                const double nrElements = 2.0;
                auto elm = alphaBeta(3.5, { 0.6, 0.8 });
                const auto inRhoT = vector1D({ 1.0, 0.5 });

                auto orig = elm;

                up.upscaleInTime(nrElements, elm, inRhoT);
                auto C = cmb.combineTwoElementsPartialCorrelation(orig, orig, inRhoT, combineAndOr::combOr);

                EXPECT_NEAR(C.ab.getBeta(), elm.getBeta(), 2e-3);
                EXPECT_EQ(C.n, 0);
                // no check on alpha values in Fortran code
            }

            // Testing of the consistency of the equivalent alpha according to the methods upscaleInTime and combineTwoElementsPartialCorrelation
            // Test with 2 fully correlated elements.
            // Beta is 3.5, the design point is equal to (1.0 0.0), and Rho is equal to (1.0 0.0)
            // The resulting beta is verified and resulting alfa's should be (approximately) equal
            void upscale_tests::EquivalentAlphaTesting3()
            {
                const double nrElements = 2.0;

                auto elm = alphaBeta(3.5, { 1.0, 0.0 });
                auto inRhoT = vector1D({ 1.0, 0.0 });

                auto ORG = elm; // copy of original elm

                up.upscaleInTime(nrElements, elm, inRhoT);
                auto C = cmb.combineTwoElementsPartialCorrelation(ORG, ORG, inRhoT, combineAndOr::combOr);

                ut.checkAlphaBeta(elm, C.ab, 1e-3);
                EXPECT_EQ(C.n, 0);
            }

            // Testing of the consistency of the equivalent alpha according to the methods upscaleInTime and combineTwoElementsPartialCorrelation
            // Test with 2 uncorrelated elements.
            // Beta is 3.5, the design point is equal to (1.0 0.0), and Rho is equal to (0.0 1.0)
            // The resulting beta is verified and resulting alfa's should be (approximately) equal
            void upscale_tests::EquivalentAlphaTesting4()
            {
                const double nrElements = 2.0;

                auto elm = alphaBeta(3.5, { 1.0, 0.0 });
                auto inRhoT = vector1D({ 0.0, 1.0 });

                auto ORG = elm; // copy of original elm

                up.upscaleInTime(nrElements, elm, inRhoT);
                auto C = cmb.combineTwoElementsPartialCorrelation(ORG, ORG, inRhoT, combineAndOr::combOr);

                ut.checkAlphaBeta(elm, C.ab, 1e-3);
                EXPECT_EQ(C.n, 0);
            }

            // Testing of the consistency of the equivalent alpha according to the methodes upscaleInTime and combineTwoElementsPartialCorrelation \n
            // Test with 2 partially correlated elements. \n
            // Beta is 3.5, the design point is equal to (0.6 -0.8), and Rho is equal to (1.0 0.5)
            // The resulting beta is verified and resulting alfa's should be (approximately) equal
            void upscale_tests::EquivalentAlphaTesting5()
            {
                const double nrElements = 2.0;

                auto elm = alphaBeta(3.5, { 0.6, -0.8 });
                auto inRhoT = vector1D({ 1.0, 0.5 });

                auto ORG = elm; // copy of original elm

                up.upscaleInTime(nrElements, elm, inRhoT);
                auto C = cmb.combineTwoElementsPartialCorrelation(ORG, ORG, inRhoT, combineAndOr::combOr);

                EXPECT_NEAR(C.ab.getBeta(), elm.getBeta(), 2e-3);
                EXPECT_EQ(C.n, 0);
                // no check on alpha values in Fortran code
            }

            // Testing of the consistency of the equivalent alpha according to the methodes upscaleInTime and combineTwoElementsPartialCorrelation
            // Test with 2 partially correlated elements.
            // Beta is 3.5, the design point is equal to (0.6 -0.8), and Rho is equal to (1.0 0.0)
            // The resulting beta is verified and resulting alfa's should be (approximately) equal
            void upscale_tests::EquivalentAlphaTesting6()
            {
                const double nrElements = 2.0;

                auto elm = alphaBeta(3.5, { 0.6, -0.8 });
                auto inRhoT = vector1D({ 1.0, 0.0 });

                auto ORG = elm; // copy of original element

                up.upscaleInTime(nrElements, elm, inRhoT);
                auto C = cmb.combineTwoElementsPartialCorrelation(ORG, ORG, inRhoT, combineAndOr::combOr);
                ut.checkAlphaBeta(elm, C.ab, 1e-3, 2e-2);
                EXPECT_EQ(C.n, 0);
            }

            // Testing the upscaling of probabilities to largest block duration.
            // Test with 5 random variables, beta=3.5, alpha is random and block durations are equal.
            // The resulting beta and alfa's should be equal to the values for the single element
            void upscale_tests::upscaleToLargestBlockTests1()
            {
                const size_t nrStochasts = 5;
                double beta = 3.5;
                auto alpha = vector1D({ 1.0, 2.0, 3.0, 4.0, 5.0 }); // alpha values
                alpha.normalize();
                auto duration = vector1D(nrStochasts);
                duration.assign(1.0);                        // individual block durations
                const auto inrhot = vector1D({ 0.1, 0.3, 0.5, 0.7, 0.9 }); // all rho values

                auto ORG = alphaBeta(beta, alpha); // Copy double of the original beta value
                const auto durationORG = duration;  // Copy of the original durations
                const double maxduration = duration.maxval();

                alphaBeta elm;
                up.upscaleToLargestBlock(ORG, inrhot, durationORG, maxduration, elm, duration);

                ut.checkAlphaBeta(elm, ORG, 0.001);
            }

            // Testing the upscaling of probabilities to largest block duration.
            // Test with 5 random variables, beta=3.5, alpha is random and block durations are equal.
            // The resulting beta and alfa's should be equal to the values for the single element
            void upscale_tests::upscaleToLargestBlockTests2()
            {
                const size_t nrStochasts = 5;
                double beta = 3.5;
                auto alpha = vector1D(nrStochasts); // equal alpha values
                alpha.assign(1.0); alpha.normalize();
                auto duration = vector1D({ 1.0, 2.0, 3.0, 4.0, 5.0 }); // individual block durations
                auto inrhot = vector1D(nrStochasts); // all rho values
                inrhot.assign(1.0);

                auto ORG = alphaBeta(beta, alpha); // Copy of the original beta and alpha values
                const auto durationORG = duration; // Copy of the original durations
                const auto maxduration = duration.maxval();

                alphaBeta elm;
                up.upscaleToLargestBlock(ORG, inrhot, durationORG, maxduration, elm, duration);

                ut.checkAlphaBeta(elm, ORG, 0.001);
            }

            // Testing the upscaling of probabilities to largest block duration.
            // Test with 5 random variables, beta=3.5, design point is equal to (0 0.6 0 0.8 0)
            // Auto-correlations are all set to zero. Block durations of variables with alpha>0 differ by a factor 10 with the maximum.
            // The resulting beta is verified (see upscaleInTimeTest3) and alfa's should be equal to the values for the single element.
            void upscale_tests::upscaleToLargestBlockTests3()
            {
                const size_t nrStochasts = 5;
                double beta = 3.5;
                auto alpha = vector1D({ 0.0, 0.6, 0.0, 0.8, 0.0 });     // alpha values
                auto duration = vector1D({ 20.0, 2.0, 1.0, 2.0, 10.0 });   // individual block durations
                const auto inrhot = vector1D(nrStochasts);               // all rho values set to zero

                const auto ORG = alphaBeta(beta, alpha); // Copy of the original beta and alpha values
                const auto durationORG = duration;  // Copy of the original durations
                const auto maxduration = duration.maxval();

                const double p = StandardNormal::getPFromU(beta);
                auto beta2 = StandardNormal::getUFromQ(1.0 - pow(p, 10));
                auto ref = alphaBeta(beta2, ORG.getAlpha());

                alphaBeta elm;
                up.upscaleToLargestBlock(ORG, inrhot, durationORG, maxduration, elm, duration);

                ut.checkAlphaBeta(elm, ref, 0.001);
            }

        }
    }
}
