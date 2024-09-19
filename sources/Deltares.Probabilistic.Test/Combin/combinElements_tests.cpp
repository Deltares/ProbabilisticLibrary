// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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
#include "gtest/gtest.h"
#include <math.h>
#include "combinElements_tests.h"
#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"
#include "../../Deltares.Probabilistic/Combine/LengthEffect.h"

using namespace Deltares::Reliability;
using namespace Deltares::Statistics;

namespace Deltares{ namespace Probabilistic { namespace Test {


 void combinElementsTests::runAllCombineTwoElementsTests()
{
    testCombineTwoElementsNoCorrelation1();
    testCombineTwoElementsNoCorrelation2();
    testCombineTwoElementsNoCorrelation3();
    testCombineTwoElementsNoCorrelation4();
    testCombineTwoElementsNoCorrelation5();
    testCombineTwoElementsNoCorrelation6();
    testcombineTwoElementsPartialCorrelation1();
    testcombineTwoElementsPartialCorrelation2();
    testcombineTwoElementsPartialCorrelation3();
    testcombineTwoElementsPartialCorrelation4();
    testcombineTwoElementsPartialCorrelation5();
    testcombineTwoElementsPartialCorrelation6();
    testcombineTwoElementsPartialCorrelation7();
    testcombineTwoElementsPartialCorrelation8();
    testcombineTwoElementsPartialCorrelation9();
    testcombineTwoElementsPartialCorrelation10();
    testcombineTwoElementsPartialCorrelation11();
    testcombineTwoElementsPartialCorrelation12();
    testcombineTwoElementsPartialCorrelation13();
    testcombineTwoElementsPartialCorrelation14();
    testcombineTwoElementsPartialCorrelation15();
    testcombineTwoElementsPartialCorrelation99();
    testcombineTwoElementsPCorWithNonContrAlphas();
    testcombineMultipleElementsProb1();
    testcombineMultipleElementsProb2();
    testcombineMultipleElementsProb3();
    testcombineMultipleElementsProb4();
    testcombineMultipleElementsProb5();
    testCombineElementsFullCorrelation1();
    testCombineElementsFullCorrelation2();
    testcombineTwoElementsNegativeCorrelation1();
    testcombineTwoElementsNegativeCorrelation2();
    testcombineThreeElementsPartialCorrelation1();
    testcombineThreeElementsPartialCorrelation2();
    testcombineThreeElementsPartialCorrelation3();
    testcombineThreeElementsPartialCorrelation4();
    testcombineTwoElementsPartialCorrelationPiping();
    testcombineTwoElementsPartialCorrelation9a();
    testcombineTwoElementsPartialCorrelation10a();
    testcombineTwoElementsPartialCorrelation11a();
    testcombineTwoElementsPartialCorrelation12a();
    testcombineTwoElementsPartialCorrelation13a();
    testcombineTwoElementsPartialCorrelation14a();
    testcombineTwoElementsPartialCorrelation99a();
}

void combinElementsTests::runAllLengthEffectTests()
{
    testUpscalingLengthNewInterface();
    testcombineMultipleElementsSpatialCorrelated1();
    testcombineMultipleElementsSpatialCorrelated2();
    testcombineMultipleElementsSpatialCorrelated3();
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, but uncorrelated elements
// Element 1 with beta= 3.0, and design point alpha= (1, 0)
// Element 2 with beta= 2.5, and design point alpha= (0, 1)
// RhoP = (0, 0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testCombineTwoElementsNoCorrelation1()
{
    const size_t nStochast = 2; // number of stochastic variables
    auto elm1 = alphaBeta(3.0, {1.0, 0.0});
    auto elm2 = alphaBeta(2.5, {0.0, 1.0});
    auto rhoP   = vector1D(nStochast); // defaults to (0.0, 0.0)

//  Compute analytically the beta that is expected for the OR-port combination of the present two elements:
    double p1 = elm1.getP();
    double p2 = elm2.getP();
    auto beta = StandardNormal::getUFromQ(1.0 - p1 * p2);
    auto ref = alphaBeta(beta, {0.246342425266045, 0.969182251096718}); // pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, but uncorrelated elements
// Element 1 with beta= 2.5, and design point alpha= (0, 1)
// Element 2 with beta= 3.0, and design point alpha= (1, 0)
// RhoP = (0, 0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. The results of this test can be compared to the preceding test to verify that the order of the elements does not affect the outcome.
void combinElementsTests::testCombineTwoElementsNoCorrelation2()
{
    auto elm1 = alphaBeta(2.5, {0.0, 1.0});
    auto elm2 = alphaBeta(3.0, {1.0, 0.0});
    auto rhoP = vector1D({0.0, 0.0});

//  Compute analytically the beta that is expected for the OR-port combination of the present two elements:
    double p1 = elm1.getP();
    double p2 = elm2.getP();
    auto beta = StandardNormal::getUFromQ(1.0 - p1 * p2);
    auto ref = alphaBeta(beta, {0.246342425266045, 0.969182251096718}); // Is somewhere pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, but uncorrelated elements
// Element 1 with beta= 2.5, and design point alpha= (1, 0)
// Element 2 with beta= 3.0, and design point alpha= (0, 1)
// RhoP = (0, 0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB: The results of this test can be compared to the two preceding ones to verify that now the alpha(1) and alpha(2)
// of the combination are in reversed order. The reason is that for the same alpha1() and alpha2() the beta1 and beta2 were reversed.
// In this way it is verified that the "beta-order" of the stochastic variables has an effect.
void combinElementsTests::testCombineTwoElementsNoCorrelation3()
{
    auto elm1 = alphaBeta(2.5, {1.0, 0.0});
    auto elm2 = alphaBeta(3.0, {0.0, 1.0});
    auto rhoP = vector1D({0.0, 0.0});

//  Compute analytically the beta that is expected for the OR-port combination of the present two elements:
    double p1 = elm1.getP();
    double p2 = elm2.getP();
    auto beta = StandardNormal::getUFromQ(1.0 - p1 * p2);
    auto ref = alphaBeta(beta, {0.969182251096718, 0.246342425266045}); // pre-computed
//   NB. The expectedAlphaC(1) and expectedAlphaC(2) must now be in reversed order compared to
//       the preceding tests testCombineTwoElementsNoCorrelation1 and testCombineTwoElementsNoCorrelation2

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the AND-combination of 2 non-identical, but uncorrelated elements
// Element 1 with beta= 2.5, and design point alpha= (1, 0)
// Element 2 with beta= 3.0, and design point alpha= (0, 1)
// RhoP = (0, 0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testCombineTwoElementsNoCorrelation4()
{
    auto elm1 = alphaBeta(2.5, {1.0, 0.0});
    auto elm2 = alphaBeta(3.0, {0.0, 1.0});
    auto rhoP = vector1D({0.0, 0.0});

//  Compute analytically the beta that is expected for the AND-port combination of the present two elements:
    double q1 = elm1.getQ();
    double q2 = elm2.getQ();
    auto beta = StandardNormal::getUFromQ(q1 * q2);
    auto ref = alphaBeta(beta, {0.651828661927589, 0.758366267373285}); // Is somewhere pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combAnd);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the AND-combination of 2 non-identical, but uncorrelated elements
// Element 1 with beta= -2.5, and design point alpha= (1, 0)
// Element 2 with beta= -3.0, and design point alpha= (0, 1)
// RhoP = (0, 0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testCombineTwoElementsNoCorrelation5()
{
    auto elm1 = alphaBeta(-2.5, {1.0, 0.0});
    auto elm2 = alphaBeta(-3.0, {0.0, 1.0});
    auto rhoP = vector1D({0.0, 0.0});

//  Compute analytically the beta that is expected for the AND-port combination of the present two elements:
    double q1 = elm1.getQ();
    double q2 = elm2.getQ();
    auto beta = StandardNormal::getUFromQ(q1 * q2);
    auto ref = alphaBeta(beta, {0.970353143592546, 0.241688910290521}); // Is somewhere pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combAnd);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the AND-combination of 2 non-identical, but uncorrelated elements
// Element 1 with beta= 0.0, and design point alpha= (1, 0)
// Element 2 with beta= 0.0, and design point alpha= (0, 1)
// RhoP = (0, 0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testCombineTwoElementsNoCorrelation6()
{
    auto elm1 = alphaBeta(0.0, {1.0, 0.0});
    auto elm2 = alphaBeta(0.0, {0.0, 1.0});
    auto rhoP = vector1D({0.0, 0.0});

//  Compute analytically the beta that is expected for the AND-port combination of the present two elements:
    double q1 = elm1.getQ();
    double q2 = elm2.getQ();
    auto beta = StandardNormal::getUFromQ(q1 * q2);
    auto ref = alphaBeta(beta, {sqrt(0.5), sqrt(0.5)});
    // The alpha()-components of the combination must be equal because of equal beta1 and beta2

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combAnd);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.0, and design point alpha= (0.6, 0.8)
// Element 2 with beta= 2.5, and design point alpha= (0.8, 0.6)
// RhoP = (0.5, 0.5)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testcombineTwoElementsPartialCorrelation1()
{
    auto elm1 = alphaBeta(3.0, {0.6, 0.8});
    auto elm2 = alphaBeta(2.5, {0.8, 0.6});
    auto rhoP   = vector1D({0.5, 0.5});

    auto ref = alphaBeta(2.43858507200259, {0.777037047950730, 0.629454864237314}); // pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.0, and design point alpha= (0.6, 0.8)
// Element 2 with beta= 2.5, and design point alpha= (0.8, 0.6)
// RhoP = (1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. With RhoP=(1, 1) the robustness of the function is tested for full correlation of the stochastic variables.
void combinElementsTests::testcombineTwoElementsPartialCorrelation2()
{
    auto elm1 = alphaBeta(3.0, {0.6, 0.8});
    auto elm2 = alphaBeta(2.5, {0.8, 0.6});
    auto rhoP = vector1D({1.0, 1.0});

    auto ref = alphaBeta(2.49715643746047, {0.795187280968291, 0.606363907390814});

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 2.5, and design point alpha= (0.6, 0.8)
// Element 2 with beta= 2.5, and design point alpha= (0.8, 0.6)
// RhoP = (1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Particularly, with this RhoP=(1, 1) the robustness of the function is tested for full correlation of the stochastic variables.
//     Because of equal beta of the two elements, the alpha(1) and alpha(2) for the combination must have the same value.
void combinElementsTests::testcombineTwoElementsPartialCorrelation3()
{
    auto elm1 = alphaBeta(2.5, {0.6, 0.8});
    auto elm2 = alphaBeta(2.5, {0.8, 0.6});
    auto rhoP   = vector1D({1.0, 1.0});

    auto ref = alphaBeta(2.38813303477200, {sqrt(0.5), sqrt(0.5)});
    // The alpha()-components of the combination must be equal because of equal beta1 and beta2

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and uncorrelated elements
// Element 1 with beta= 2.5, and design point alpha= (-0.6, 0.8)
// Element 2 with beta= 2.5, and design point alpha= ( 0.8, 0.6)
// RhoP = (1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Fully correlated stochastic variables, but uncorrelated elements.
// NB. A negative value is assigned to alpha1(1) of the first element.
void combinElementsTests::testcombineTwoElementsPartialCorrelation4()
{
    auto elm1 = alphaBeta(2.5, {-0.6, 0.8});
    auto elm2 = alphaBeta(2.5, {0.8, 0.6});
    auto rhoP   = vector1D({1.0, 1.0});

//  Because of these alpha() and RhoP() the two elements are uncorrelated. The combined beta can then be derived analytically:
    double p1 = elm1.getP();
    double p2 = elm2.getP();

    auto beta = StandardNormal::getUFromQ(1.0 - p1 * p2);
    auto ref = alphaBeta(beta, {0.149904324474973, 0.988700507486318}); // pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and correlated elements
// Element 1 with beta= 2.5, and design point alpha= (-0.6, 0.8)
// Element 2 with beta= 2.5, and design point alpha= (-0.8, 0.6)
// RhoP = (1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Negative values are assigned to alpha1(1) of the first element and alpha2(1) of the second element.
//     It is verified that the alphaC(1) of the combination is also negative.
void combinElementsTests::testcombineTwoElementsPartialCorrelation5()
{
    auto elm1 = alphaBeta(2.5, {-0.6, 0.8});
    auto elm2 = alphaBeta(2.5, {-0.8, 0.6});
    auto rhoP   = vector1D({1.0, 1.0});

    auto ref = alphaBeta(2.38813303477200, {-0.713029134520995, 0.701134404607448});
    //expectedAlphaC= sqrt(0.5d0) * (/ -1.0d0, 1.0d0 /)   ! theoretically expected, but not exactly (re)produced

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and uncorrelated elements
// Element 1 with beta= 2.5, and design point alpha= (-0.5,  0.5, -0.5, 0.5)
// Element 2 with beta= 2.5, and design point alpha= (-0.5, -0.5,  0.5, 0.5)
// RhoP = (0.9, 0.9, 0.9, 0.9)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present number of stochastic variables is 4
// NB. Correlated stochastic variables, but uncorrelated elements.
// NB. Positive and negative values in the alpha1() and alpha2().
void combinElementsTests::testcombineTwoElementsPartialCorrelation6()
{
    auto elm1 = alphaBeta(2.5, {-0.5,  0.5, -0.5, 0.5});
    auto elm2 = alphaBeta(2.5, {-0.5, -0.5,  0.5, 0.5});
    auto rhoP = vector1D({0.9, 0.9, 0.9, 0.9});

//  For these alpha() and RhoP() the two elements are uncorrelated, and beta of the present OR-combination can be derived analytically:
    double p1 = elm1.getP();
    double p2 = elm2.getP();

    auto beta = StandardNormal::getUFromQ(1.0 - p1 * p2);
    auto ref = alphaBeta(beta, {-0.689062334733955, -0.158849035402622, 0.157472623165194, 0.689320285321795}); // pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 2.5, and design point alpha= (-1.0)
// Element 2 with beta= 2.5, and design point alpha= ( 1.0)
// RhoP = ( 0.9 )
// The resulting beta of the combination should be equal to its expected value
// The alpha of the combination of the these two elements is not well defined and is not verified
// NB. Merely one stochastic variable in the elements
// NB. Negative correlation of the two elements.
void combinElementsTests::testcombineTwoElementsPartialCorrelation7()
{
    auto elm1 = alphaBeta(2.5, {-1.0});
    auto elm2 = alphaBeta(2.5, { 1.0});
    auto rhoP = vector1D({0.9});

    double expectedBeta = 2.24390055691005;

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    ASSERT_NEAR(elm3.ab.getBeta(), expectedBeta, 2.5e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 2.5, and design point alpha= ( 1.0)
// Element 2 with beta= 2.5, and design point alpha= (-1.0)
// RhoP = ( 0.9 )
// The resulting beta of the combination should be equal to its expected value
// The alpha of the combination of the these two elements is not well defined and is not verified
// NB. Merely one stochastic variable in the elements
// NB. Negative correlation of the two elements.
void combinElementsTests::testcombineTwoElementsPartialCorrelation8()
{
    auto elm1 = alphaBeta(2.5, { 1.0});
    auto elm2 = alphaBeta(2.5, {-1.0});
    auto rhoP = vector1D({0.9});

    const double expectedBeta  = 2.24390055691005;

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    ASSERT_NEAR(elm3.ab.getBeta(), expectedBeta, 2.5e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= Normalised( 0.436, 0.9  )
// Element 2 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.436)
// RhoP = ( 1.0, 0.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 3: Standard case with different alpha() values.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013.
//     See also the associated test in testcombineTwoElementsPartialCorrelation10
void combinElementsTests::testcombineTwoElementsPartialCorrelation9()
{
    auto elm1 = alphaBeta(3.5, {0.436, 0.900});
    auto elm2 = alphaBeta(3.5, {0.900, 0.436});
    elm1.normalize();
    elm2.normalize();
    auto rhoP = vector1D({1.0, 0.0});

    auto ref = alphaBeta(3.31256284734426, {0.764786211092966, 0.644284138654728}); // pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= Normalised( 0.436, 0.9, 0.0  )
// Element 2 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.0, 0.436)
// RhoP = ( 1.0, 1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 3: Standard case with different alpha() values.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
//     See also the associated test in testcombineTwoElementsPartialCorrelation9
void combinElementsTests::testcombineTwoElementsPartialCorrelation10()
{
    auto elm1 = alphaBeta(3.5, {0.436, 0.900, 0.000});
    auto elm2 = alphaBeta(3.5, {0.900, 0.000, 0.436});
    elm1.normalize();
    elm2.normalize();
    auto rhoP = vector1D({1.0, 1.0, 1.0});

    auto ref = alphaBeta(3.31256284734426,
    {0.764786211092966, 0.580277735143150, 0.279963928782266}); // pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8  )
// Element 2 with beta= 3.5, and design point alpha= ( 0.6, 0.8  )
// RhoP = ( 1.0, 0.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 1: Standardcase.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
//     See also the associated test in testcombineTwoElementsPartialCorrelation12
void combinElementsTests::testcombineTwoElementsPartialCorrelation11()
{
    auto elm1 = alphaBeta(3.5, {0.6, 0.8});
    auto elm2 = elm1;
    auto rhoP = vector1D({1.0, 0.0});

    auto ref = alphaBeta(3.3121491224946, {0.634952576684642, 0.772551115047758}); // pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8, 0.0 )
// Element 2 with beta= 3.5, and design point alpha= ( 0.6, 0.0, 0.8 )
// RhoP = ( 1.0, 1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 1: Standardcase.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
//     See also the associated test in testcombineTwoElementsPartialCorrelation11
void combinElementsTests::testcombineTwoElementsPartialCorrelation12()
{
    auto elm1 = alphaBeta(3.5, {0.6, 0.8, 0.0});
    auto elm2 = alphaBeta(3.5, {0.6, 0.0, 0.8});
    auto rhoP = vector1D({1.0, 1.0, 1.0});

    auto ref = alphaBeta(3.31214912249458,
    {0.634952576684642, 0.546276132263498, 0.546276132263498}); // pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8  )
// Element 2 with beta= 3.8, and design point alpha= ( 0.6, 0.8  )
// RhoP = ( 1.0, 0.0 )
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 2: Standardcase with different beta-values.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
//     See also the associated test in testcombineTwoElementsPartialCorrelation14
void combinElementsTests::testcombineTwoElementsPartialCorrelation13()
{
//  For "old" combineTwoElementsPartialCorrelation, without correction of the alpha_equivalent
    auto elm1 = alphaBeta(3.5, {0.6, 0.8});
    auto elm2 = alphaBeta(3.8, {0.6, 0.8});
    auto rhoP = vector1D({1.0, 0.0});

    auto ref = alphaBeta(3.42801564331459, {0.631154181145020, 0.775657398355201}); // pre-computed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 3 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8, 0.0 )
// Element 2 with beta= 3.8, and design point alpha= ( 0.6, 0.0, 0.8 )
// RhoP = ( 1.0, 1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 2: Standardcase with different beta-values.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
//     See also the associated test in testcombineTwoElementsPartialCorrelation13
void combinElementsTests::testcombineTwoElementsPartialCorrelation14()
{
//  For "old" combineTwoElementsPartialCorrelation, without correction of the alpha_equivalent
    auto elm1 = alphaBeta(3.5, {0.6, 0.8, 0.0});
    auto elm2 = alphaBeta(3.8, {0.6, 0.0, 0.8});
    auto rhoP = vector1D({1.0, 1.0, 1.0});

    auto ref = alphaBeta(3.42801564331459,
    {0.631154181145020, 0.735508613458735, 0.246315811817203}); // pre-computed
//  expectedAlphaC2EQ= sqrt( expectedAlphaC(2)**2 + expectedAlphaC(3)**2 )

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);
    //double expectedAlphaC2EQ = 0.727073922142432; //  TODO not used/checked
    //auto alphaC2EQ = hypot(elm3.alpha(2-1), elm3.alpha(3-1));
    //auto dAlphaC2EQ = alphaC2EQ - expectedAlphaC2EQ;

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= ( 0.8, 0.6 )
// Element 2 with beta= 9.5, and design point alpha= ( 0.8, 0.6 )
// RhoP = ( 1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testcombineTwoElementsPartialCorrelation15()
{
    auto elm1 = alphaBeta(3.5, {0.8, 0.6});
    auto elm2 = alphaBeta(9.5, {0.8, 0.6});
    auto rhoP = vector1D({1.0, 0.0});

    auto ref = elm1; // approx.

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.0, and dimension 10 of design point alpha
// Element 2 with beta= 3.5, and dimension 10 of design point alpha
// RhoP = ( 1.0, ..., 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testcombineTwoElementsPartialCorrelation99()
{
    const size_t nStochast = 10;
    auto elm1 = alphaBeta(3.0, {0.6, 0.8, 0.4, 0.0, 0.2, 0.7, 0.3, 0.8, 0.9, 1.0});
    auto elm2 = alphaBeta(3.5, {0.1, 0.0, 0.7, 0.9, 0.8, 0.1, 0.9, 0.5, 0.3, 0.0});
    auto rhoP = vector1D(nStochast);
    rhoP.assign(1.0);
    elm1.normalize();
    elm2.normalize();

//  Expected beta and alpha():
    auto ref = alphaBeta(2.95370049889346,
      {0.277865981946082, 0.361174823684065, 0.244040175818021,
       0.906352706365079e-1, 0.168592815519657, 0.323038934343359,
       0.222247930634836, 0.395033459584985, 0.427126699802553,
       0.451566604194081}); // PreComputed

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 0);
}

// Test of combineTwoElementsPartialCorrelation \n
// Testing the OR-combination of 2 non-identical, and partially correlated elements \n
// Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8  ) \n
// Element 2 with beta= 3.8, and design point alpha= ( 0.6, 0.8  ) \n
// RhoP = ( 1.0, 0.0 ) \n
// The resulting beta and alphas of the combination should be equal to their expected values \n
// NB. Present test is as described in memo: \n
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
//     Section 3. Onnauwkeurigheid alfa-bepaling. \n
//     Testcase 2: Standardcase with different beta-values. \n
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
//     See also the associated test in testcombineTwoElementsPartialCorrelation13 \n
void combinElementsTests::testcombineTwoElementsPCorWithNonContrAlphas()
{
    auto elm1 = alphaBeta(3.5, {0.0, 0.6, 0.0, 0.8, 0.0});
    auto elm2 = alphaBeta(3.8, elm1.getAlpha());
    auto rhoP = vector1D({0.0, 1.0, 0.5, 0.0, 1.0});

    auto ref = alphaBeta(3.42801564331459,
    {0.0, 0.631154181145020, 0.0, 0.775657398355201, 0.0}); // pre-computed and based on testcombineTwoElementsPartialCorrelation13

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-9);
    EXPECT_EQ(elm3.n, 0);
}

// > test testcombineMultipleElementsProb
void combinElementsTests::testcombineMultipleElementsProb1()
{
    auto Elements = elements(1);
    Elements[0] = {3.5, {1.0, 0.0, 0.0}};
    std::vector<double> percentages = {100.0};

    auto Expected = Elements[0];

    auto ab = cmb.combineMultipleElementsProb(Elements, percentages, combineAndOr::combOr);

    utils.checkAlphaBeta(ab.ab, Expected, 1e-6);
    EXPECT_EQ(ab.n, 0);
}

// test testcombineMultipleElementsProb
void combinElementsTests::testcombineMultipleElementsProb2()
{
    const size_t nElements = 2;

    auto Elements = elements(nElements);
    auto percentages = std::vector<double>(nElements);
    for (size_t i = 0; i < nElements; i++)
    {
        Elements[i] = alphaBeta(3.5, {1.0, 0.0, 0.0});
        percentages[i]    = 50.0;
    }

    auto ref = Elements[0];
    ref.setBeta(3.5000155); // precomputed; slightly different from 3.5 due to term pfu * pf2pf1

    auto ab = cmb.combineMultipleElementsProb(Elements, percentages, combineAndOr::combOr );

    utils.checkAlphaBeta(ab.ab, ref, 1e-7);
    EXPECT_EQ(ab.n, 0);
}

// test testcombineMultipleElementsProb
void combinElementsTests::testcombineMultipleElementsProb3()
{
    const size_t nElements = 10;

    auto Elements = elements(nElements);
    auto percentages = std::vector<double>(nElements);
    for (size_t i = 0; i < nElements; i++)
    {
        Elements[i] = alphaBeta(3.5, {1.0, 0.0, 0.0});
        percentages[i] = 10.0;
    }
    
    auto ref = Elements[0];
    ref.setBeta(3.5000279); // precomputed; slightly different from 3.5 due to term pfu * pf2pf1

    auto ab = cmb.combineMultipleElementsProb(Elements, percentages, combineAndOr::combOr);

    utils.checkAlphaBeta(ab.ab, ref, 1e-7);
    EXPECT_EQ(ab.n, 0);
}

// test testcombineMultipleElementsProb
void combinElementsTests::testcombineMultipleElementsProb4()
{
    const size_t nElements = 2;

    auto Elements = elements(nElements);
    Elements[0] = alphaBeta(3.0, {1.0, 0.0, 0.0});
    Elements[1] = alphaBeta(3.5, Elements[0].getAlpha());
    const auto percentages = std::vector<double>({30.0, 70.0});

    auto ref = alphaBeta(3.25457554349, Elements[0].getAlpha()); // pre-computed with Excel

    auto ab = cmb.combineMultipleElementsProb(Elements, percentages, combineAndOr::combOr);

    utils.checkAlphaBeta(ab.ab, ref, 1e-4);
    EXPECT_EQ(ab.n, 0);
}

// test testcombineMultipleElementsProb
void combinElementsTests::testcombineMultipleElementsProb5()
{
    const size_t nElements = 10;

    auto Elements = elements(nElements);
    auto percentages = std::vector<double>(nElements);
    for (size_t i = 0; i < nElements; i++)
    {
        Elements[i] = alphaBeta(3.0 + 0.1 * (double)i, {1.0, 0.0, 0.0});
        percentages[i] = 10.0;
    }
    
    auto ref = alphaBeta(3.32348724653, Elements[0].getAlpha()); // pre-computed with Excel

    auto ab = cmb.combineMultipleElementsProb(Elements, percentages, combineAndOr::combOr);

    utils.checkAlphaBeta(ab.ab, ref, 1e-4);
    EXPECT_EQ(ab.n, 0);
}

// Test of combine multiple elements spatial correlated
// This test gives the results as calculated with the method residual correlation
void combinElementsTests::testcombineMultipleElementsSpatialCorrelated1()
{
    const int nElements    = 20; // Number of elements

    auto CrossSection = alphaBeta(5.0, {0.6, sqrt(0.5 - 0.36), 0.6, sqrt(0.5 - 0.36)});
    auto rhoXK = vector1D({0.5, 0.5, 0.2, 0.2});
    auto dXK   = vector1D({500.0, 300.0, 500.0, 300.0});
    double sectionLength = 100.0;

    auto ref = alphaBeta(4.38787743765301, // pre-computed
    {0.635285167139092, 0.393519326675495, 0.565059833788674, 0.349660807332817}); // pre-computed

    auto section = up.upscaleLength(CrossSection, rhoXK, dXK, sectionLength, -999.0);
    EXPECT_EQ(section.second, 0);
    auto Elements = elements(nElements);
    for (size_t i = 0; i < nElements; i++)
    {
        Elements[i] = section.first;
    }

    auto ab = cmb.combineMultipleElements( Elements, rhoXK, combineAndOr::combOr );

    utils.checkAlphaBeta(ab.ab, ref, 1e-6);
    EXPECT_EQ(ab.n, 0);
}

// Test of combine multiple elements spatial correlated
// This test gives the results as calculated with the method residual correlation
void combinElementsTests::testcombineMultipleElementsSpatialCorrelated2()
{
    const size_t nStochast = 4; // Number of stochastic variables
    const size_t nElements = 20; // Number of elements
    auto crossSection = alphaBeta(5.0, {0.6, sqrt(0.5 - 0.36), 0.6, sqrt(0.5 - 0.36)});
    auto rhoXK = vector1D({0.5, 0.5, 0.2, 0.2});
    auto dXK   = vector1D({500.0, 300.0, 500.0, 300.0});
    double sectionLength = 100.0;

    auto ref = alphaBeta(4.61415014812874,
    {0.597708361676096, 0.375385677437277, 0.599181645055476, 0.377904305064187}); // pre-computed

    auto section = up.upscaleLength(crossSection, rhoXK, dXK, sectionLength, -999.0);
    EXPECT_EQ(section.second, 0);
    auto elms = elements(nElements);
    for (size_t i = 0; i < nElements; i++)
    {
        elms[i] = section.first;
    }

    auto rhoXAK = vector1D(nStochast);
    for (size_t i = 0; i < nStochast; i++)
    {
        rhoXAK(i) = rhoXK(i) + (1.0 - rhoXK(i)) * exp(- pow(sectionLength,2) / pow(dXK(i),2) );
    }

    auto C = cmb.combineMultipleElements(elms, rhoXAK, combineAndOr::combOr);
    utils.checkAlphaBeta(C.ab, ref, 1e-6);
    EXPECT_EQ(C.n, 0);
}

// Test of combine multiple elements spatial correlated
// This test gives the results as calculated with the method residual correlation
void combinElementsTests::testcombineMultipleElementsSpatialCorrelated3()
{
    const size_t nElements = 20; // Number of elements
    auto crossSection = alphaBeta(5.0, {0.6, sqrt(0.5 - 0.36), 0.6, sqrt(0.5 - 0.36)});
    auto rhoXK = vector1D({0.5, 0.5, 0.2, 0.2});
    auto dXK   = vector1D({500.0, 300.0, 500.0, 300.0});
    double sectionLength = 100.0;

    auto ref = alphaBeta(4.5064103581966,
    {0.578741673689891, 0.385418150246354, 0.598199853682860, 0.398331344045516}); // pre-computed

    auto section = up.upscaleLength(crossSection, rhoXK, dXK, nElements * sectionLength, -999.0);

    utils.checkAlphaBeta(section.first, ref, 1e-6);
    EXPECT_EQ(section.second, 0);
}

void combinElementsTests::testUpscalingLengthNewInterface()
{
    const int nStochasts = 4;
    std::vector< std::shared_ptr<Stochast>> stochasts;
    for (size_t i = 0; i <= nStochasts; i++)
    {
        auto s = std::make_shared<Stochast>();
        stochasts.push_back(s);
    }

    auto section = DesignPoint();
    section.Beta = 5.0;
    auto alphaValues = std::vector<double>({ 0.6, sqrt(0.5 - 0.36), 0.6, sqrt(0.5 - 0.36) });
    for (int i = 0; i < alphaValues.size(); i++ )
    {
        auto alphaValue = alphaValues[i];
        auto alpha = std::make_shared<StochastPointAlpha>();
        alpha->Alpha = alphaValue;
        alpha->Stochast = stochasts[i];
        section.Alphas.push_back(alpha);
    }

    auto rhoXK = std::vector<double>({ 0.5, 0.5, 0.2, 0.2 });
    auto rho = std::make_shared<SelfCorrelationMatrix>();
    for (size_t i = 0; i < nStochasts; i++)
    {
        rho->setSelfCorrelation(section.Alphas[i]->Stochast, rhoXK[i]);
    }

    auto dp = LengthEffect::UpscaleLength(section, rho, { 500.0, 300.0, 500.0, 300.0 }, 2000.0, -999.0);

    auto ref = alphaBeta(4.5064103581966,
        { 0.578741673689891, 0.385418150246354, 0.598199853682860, 0.398331344045516 }); // pre-computed
    EXPECT_NEAR(dp.Beta, ref.getBeta(), 1e-6);
    for (size_t i = 0; i < nStochasts; i++)
    {
        EXPECT_NEAR(dp.Alphas[i]->Alpha, ref.getAlphaI(i), 1e-6);
    }
}

void combinElementsTests::testCombineElementsFullCorrelation(const combineAndOr andOr)
{
    const size_t nrElements = 10;
    auto Elements = elements(nrElements);
    for (size_t i = 0; i < nrElements; i++)
    {
        Elements[i] = {3.0 + 0.1 * (double)i, {0.8, 0.6, 0.0, 0.0}};
    }

    auto ab = cmb.combineMultipleElementsFull(Elements, andOr);

    auto ref = alphaBeta((andOr == combineAndOr::combOr ? 3.0 : 3.9), Elements[0].getAlpha());
    utils.checkAlphaBeta(ab.ab, ref, 1e-6);
    EXPECT_EQ(ab.n, 0);
}

void combinElementsTests::testCombineElementsFullCorrelation1()
{
    testCombineElementsFullCorrelation(combineAndOr::combOr);
}

void combinElementsTests::testCombineElementsFullCorrelation2()
{
    testCombineElementsFullCorrelation(combineAndOr::combAnd);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 fully negatively correlated elements
// Element 1 with beta= 3.0, and design point alpha= ( 1.0, 0.0)
// Element 2 with beta= 2.5, and design point alpha= (-1.0, 0.0)
// RhoP = (1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testcombineTwoElementsNegativeCorrelation1()
{
    auto elm1 = alphaBeta(3.0, { 1.0, 0.0});
    auto elm2 = alphaBeta(2.5, {-1.0, 0.0});
    auto rhoP = vector1D({1.0, 1.0});

//  For an OR combination the resulting beta can analytically be computed
    double q1 = elm1.getQ();
    double q2 = elm2.getQ();

//  Probability of failure is then, corresponding beta:
    auto beta = StandardNormal::getUFromQ(q1 + q2);
    auto ref = alphaBeta(beta, elm2.getAlpha());

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 2);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the AND-combination of 2 fully negatively correlated elements
// Element 1 with beta= 3.0, and design point alpha= ( 1.0, 0.0)
// Element 2 with beta= 2.5, and design point alpha= (-1.0, 0.0)
// RhoP = (1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testcombineTwoElementsNegativeCorrelation2()
{
    auto elm1 = alphaBeta(3.0, { 1.0, 0.0});
    auto elm2 = alphaBeta(2.5, {-1.0, 0.0});
    auto rhoP = vector1D({1.0, 1.0});

//  For the present alpha and beta it can be analytically shown that no failure can occur
//  Theoretically the expected beta of the combination is then +infinity. Practically:
    auto ref = alphaBeta(40.0, {sqrt(0.5), sqrt(0.5)});

    auto elm3 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combAnd);

    utils.checkAlphaBeta(elm3.ab, ref, 1e-6);
    EXPECT_EQ(elm3.n, 2);
}

// Test of combineThreeElementsPartialCorrelation
// Testing the OR-combination of 3 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= Normalised( 0.436, 0.9, 0.0  , 0.0)
// Element 2 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.0, 0.436, 0.0)
// Element 3 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.0, 0.0, 0.436)
// RhoP = ( 1.0, 1.0, 1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testcombineThreeElementsPartialCorrelation1()
{
    auto elm1 = alphaBeta(3.5, {0.436, 0.900, 0.000, 0.000});
    auto elm2 = alphaBeta(3.5, {0.900, 0.000, 0.436, 0.000});
    auto elm3 = alphaBeta(3.5, {0.900, 0.000, 0.000, 0.436});
    auto rhoP = vector1D({1.0, 1.0, 1.0, 1.0});
    elm1.normalize();
    elm2.normalize();
    elm3.normalize();

    auto ref = alphaBeta(3.21126288603446,
    {0.833189348302311, 0.471747264980611, 0.227197897520706, 0.177851463927219}); // pre-computed

    auto C12 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);
    EXPECT_EQ(C12.n, 0);
    auto C   = cmb.combineTwoElementsPartialCorrelation(C12.ab, elm3, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(C.ab, ref, 1e-6);
    EXPECT_EQ(C.n, 0);
}

// Test of combineThreeElementsPartialCorrelation
// Testing the OR-combination of 3 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.6, 0.0, 0.0)
// Element 2 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.6, 0.0)
// Element 3 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.0, 0.6)
// RhoP = ( 1.0, 1.0, 1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testcombineThreeElementsPartialCorrelation2()
{
    auto elm1 = alphaBeta(3.5, {0.8, 0.6, 0.0, 0.0});
    auto elm2 = alphaBeta(3.5, {0.8, 0.0, 0.6, 0.0});
    auto elm3 = alphaBeta(3.5, {0.8, 0.0, 0.0, 0.6});
    auto rhoP = vector1D({1.0, 1.0, 1.0, 1.0});
    elm1.normalize();
    elm2.normalize();
    elm3.normalize();

    auto ref = alphaBeta(3.21478388977581,
    {0.844702211809561, 0.330698381721459, 0.330698381721459, 0.260298547859284}); // pre-computed

    auto C12 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);
    EXPECT_EQ(C12.n, 0);
    auto C   = cmb.combineTwoElementsPartialCorrelation(C12.ab, elm3, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(C.ab, ref, 1e-6);
    EXPECT_EQ(C.n, 0);
}

// Test of combineThreeElementsPartialCorrelation
// Testing the OR-combination of 3 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.6, 0.0, 0.0)
// Element 2 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.6, 0.0)
// Element 3 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.0, 0.6)
// RhoP = ( 1.0, 1.0, 1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testcombineThreeElementsPartialCorrelation3()
{
    auto elm1 = alphaBeta(3.3, {1.0, 0.0, 0.0});
    auto elm2 = alphaBeta(3.3, {0.0, 1.0, 0.0});
    auto elm3 = alphaBeta(3.5, {0.0, 0.0, 1.0});
    auto rhoP = vector1D({1.0, 1.0, 1.0});
    elm1.normalize();
    elm2.normalize();
    elm3.normalize();

// test
    double p1 = elm1.getP();
    double p2 = elm2.getP();
    double p3 = elm3.getP();
    double betaC = StandardNormal::getUFromQ(1.0 - p1 * p2 * p3);
//
    auto ref = alphaBeta(betaC, {0.730696091970909, 0.572806816016527, 0.371450632929127}); // pre-computed

    auto C12 = cmb.combineTwoElementsPartialCorrelation(elm1, elm3, rhoP, combineAndOr::combOr);
    EXPECT_EQ(C12.n, 0);
    auto C   = cmb.combineTwoElementsPartialCorrelation(C12.ab, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(C.ab, ref, 1e-6);
    EXPECT_EQ(C.n, 0);
}

// Test of combineThreeElementsPartialCorrelation
// Testing the OR-combination of 3 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.6, 0.0, 0.0)
// Element 2 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.6, 0.0)
// Element 3 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.0, 0.6)
// RhoP = ( 1.0, 1.0, 1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
void combinElementsTests::testcombineThreeElementsPartialCorrelation4()
{
    auto elm1 = alphaBeta(3.3, {0.8, 0.5, 0.33});
    auto elm2 = alphaBeta(3.3, {0.6, 0.7, 0.39});
    auto elm3 = alphaBeta(3.5, {0.6, 0.5, 0.63});
    auto rhoP = vector1D({1.0, 0.7, 0.0});
    elm1.normalize();
    elm2.normalize();
    elm3.normalize();

    auto ref = alphaBeta(3.06335717484026,
    {0.711080695307172, 0.613705694896886, 0.343117421318999}); //   ! pre-computed

    auto C12 = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);
    EXPECT_EQ(C12.n, 0);
    auto C   = cmb.combineTwoElementsPartialCorrelation(C12.ab, elm3, rhoP, combineAndOr::combOr);
    EXPECT_EQ(C.n, 0);

    C12 = cmb.combineTwoElementsPartialCorrelation(elm1, elm3, rhoP, combineAndOr::combOr);
    EXPECT_EQ(C12.n, 0);
    C   = cmb.combineTwoElementsPartialCorrelation(C12.ab, elm2, rhoP, combineAndOr::combOr);
    EXPECT_EQ(C.n, 0);

    C12 = cmb.combineTwoElementsPartialCorrelation(elm2, elm3, rhoP, combineAndOr::combOr);
    EXPECT_EQ(C12.n, 0);
    C   = cmb.combineTwoElementsPartialCorrelation(C12.ab, elm1, rhoP, combineAndOr::combOr);
    EXPECT_EQ(C.n, 0);

    utils.checkAlphaBeta(C.ab, ref, 1e-6);
}

// test to reproduce behaviour of issue: HRING-1329
// beta < 0 and rho=1 for most important alpha value
// condensed alpha's from a crashing piping calculation
void combinElementsTests::testcombineTwoElementsPartialCorrelationPiping()
{
    auto elm1 = alphaBeta(-0.672801079367189, {0.955916666381498, 0.269308913244088, 0.117030065288253});
    auto elm2 = alphaBeta(-1.66664947542178, {0.941548934494231, 0.336876242987242, 2.938671066518333e-5});
    auto rhoP = vector1D({1.0, 1.0, 0.0});

    auto C = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    auto ref = alphaBeta(-1.66664950156622, {0.941544051697653, 0.336889889798984, 2.939378451871372e-5});

    utils.checkAlphaBeta(C.ab, ref, 1e-6);
    EXPECT_EQ(C.n, 0);
}

// Test of combineTwoElementsPartialCorrelation \n
// Testing the OR-combination of 2 non-identical, and partially correlated elements \n
// Element 1 with beta= 3.5, and design point alpha= Normalised( 0.436, 0.9, 0.0  ) \n
// Element 2 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.0, 0.436) \n
// RhoP = ( 1.0, 1.0, 1.0) \n
// The resulting beta and alphas of the combination should be equal to their expected values \n
// NB. Present test is as described in memo: \n
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
//     Section 3. Onnauwkeurigheid alfa-bepaling. \n
//     Testcase 3: Standard case with different alpha() values. \n
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
// See also the associated test in testcombineTwoElementsPartialCorrelation9() \n
void combinElementsTests::testcombineTwoElementsPartialCorrelation9a()
{
    auto elm1 = alphaBeta(3.5, {0.436, 0.9, 0.0});
    auto elm2 = alphaBeta(3.5, {0.9, 0.0, 0.436});
    elm1.normalize();
    elm2.normalize();
    auto rhoP = vector1D({1.0, 1.0, 1.0});

    auto ref = alphaBeta(3.31256284734426,
    {0.764785923688505, 0.580278077398837, 0.279964004504562}); // pre-computed with alpha() correction

    auto C = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(C.ab, ref, 1e-6);
    EXPECT_EQ(C.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= Normalised( 0.436, 0.9  )
// Element 2 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.436)
// RhoP = ( 1.0, 0.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 3: Standard case with different alpha() values.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
// See also the associated test in testcombineTwoElementsPartialCorrelation10()
void combinElementsTests::testcombineTwoElementsPartialCorrelation10a()
{
    auto elm1 = alphaBeta(3.5, {0.436, 0.9});
    auto elm2 = alphaBeta(3.5, {0.9, 0.436});
    elm1.normalize();
    elm2.normalize();
    auto rhoP = vector1D({1.0, 0.0});

    auto ref = alphaBeta(3.31256284734426,
    {0.764785923688505, 0.64428447981301}); // pre-computed with alpha() correction

    auto C = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(C.ab, ref, 1e-6);
    EXPECT_EQ(C.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8  )
// Element 2 with beta= 3.5, and design point alpha= ( 0.6, 0.8  )
// RhoP = ( 1.0, 0.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 1: Standardcase.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
// See also testcombineTwoElementsPartialCorrelation11()
void combinElementsTests::testcombineTwoElementsPartialCorrelation11a()
{
    auto elm1 = alphaBeta(3.5, {0.6, 0.8});
    auto elm2 = elm1;
    auto rhoP = vector1D({1.0, 0.0});

    auto ref = alphaBeta(3.31214912249458,
    {0.634952167871736, 0.772551451047102}); // pre-computed with alpha() correction

    auto C = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(C.ab, ref, 1e-9);
    EXPECT_EQ(C.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8, 0.0 )
// Element 2 with beta= 3.5, and design point alpha= ( 0.6, 0.0, 0.8 )
// RhoP = ( 1.0, 1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 1: Standardcase.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
// See also testcombineTwoElementsPartialCorrelation12()
void combinElementsTests::testcombineTwoElementsPartialCorrelation12a()
{
    auto elm1 = alphaBeta(3.5, {0.6, 0.8, 0.0});
    auto elm2 = alphaBeta(3.5, {0.6, 0.0, 0.8});
    auto rhoP = vector1D({1.0, 1.0, 1.0});

    auto ref = alphaBeta(3.31214912249458,
    {0.634952167871736, 0.546276369850913, 0.546276369850913}); // pre-computed with alpha() correction

    auto C = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(C.ab, ref, 1e-9);
    EXPECT_EQ(C.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8  )
// Element 2 with beta= 3.8, and design point alpha= ( 0.6, 0.8  )
// RhoP = ( 1.0, 0.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 2: Standardcase with different beta-values.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
//     See also the associated test in testcombineTwoElementsPartialCorrelation13()
void combinElementsTests::testcombineTwoElementsPartialCorrelation13a()
{
    auto elm1 = alphaBeta(3.5, {0.6, 0.8});
    auto elm2 = alphaBeta(3.8, {0.6, 0.8});
    auto rhoP = vector1D({1.0, 0.0});

    auto ref = alphaBeta(3.42801564331459,
    {0.631154181145020, 0.775657398355201}); // pre-computed with alpha() correction

    auto C = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(C.ab, ref, 1e-9);
    EXPECT_EQ(C.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8, 0.0 )
// Element 2 with beta= 3.8, and design point alpha= ( 0.6, 0.0, 0.8 )
// RhoP = ( 1.0, 1.0, 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 2: Standardcase with different beta-values.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
//     See also the associated test in testcombineTwoElementsPartialCorrelation14
void combinElementsTests::testcombineTwoElementsPartialCorrelation14a()
{
    auto elm1 = alphaBeta(3.5, {0.6, 0.8, 0.0});
    auto elm2 = alphaBeta(3.8, {0.6, 0.0, 0.8});
    auto rhoP = vector1D({1.0, 1.0, 1.0});

    auto ref = alphaBeta(3.42801564331459,
    {0.631154181145020, 0.735508613458735, 0.246315811817203}); // pre-computed with alpha() correction

    auto C = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(C.ab, ref, 1e-9);
    EXPECT_EQ(C.n, 0);
}

// Test of combineTwoElementsPartialCorrelation
// Testing the OR-combination of 2 non-identical, and partially correlated elements
// Element 1 with beta= 3.0, and dimension 10 of design point alpha
// Element 2 with beta= 3.5, and dimension 10 of design point alpha
// RhoP = ( 1.0, ..., 1.0)
// The resulting beta and alphas of the combination should be equal to their expected values
// NB. Present test is for a correction of the alpha in combineElements() as described in memo:
//     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.
//     Section 3. Onnauwkeurigheid alfa-bepaling.
//     Testcase 2: Standardcase with different beta-values.
//     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
//     See also the associated test in testcombineTwoElementsPartialCorrelation99
void combinElementsTests::testcombineTwoElementsPartialCorrelation99a()
{
    auto elm1 = alphaBeta(3.0, {0.6, 0.8, 0.4, 0.0, 0.2, 0.7, 0.3, 0.8, 0.9, 1.0});
    auto elm2 = alphaBeta(3.5, {0.1, 0.0, 0.7, 0.9, 0.8, 0.1, 0.9, 0.5, 0.3, 0.0});
    elm1.normalize();
    elm2.normalize();
    auto rhoP = vector1D(10);
    rhoP.assign(1.0);

//  Expected beta and alpha() when the alpha() correction in combineElements() is used:
    auto ref = alphaBeta(2.95370049889346,
    {0.277865981946082, 0.361174823684065, 0.244040175818021,
     0.906352706365079e-1, 0.168592815519657, 0.323038934343359,
     0.222247930634836, 0.395033459584985, 0.427126699802553,
     0.451566604194081}); // PreComputed

    auto C = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rhoP, combineAndOr::combOr);

    utils.checkAlphaBeta(C.ab, ref, 1e-9);
    EXPECT_EQ(C.n, 0);
}}}}
