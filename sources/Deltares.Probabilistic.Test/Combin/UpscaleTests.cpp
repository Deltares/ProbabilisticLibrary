// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include <cmath>
#include <gtest/gtest.h>
#include "UpscaleTests.h"
#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"

namespace Deltares::Probabilistic::Test
{
    void UpscaleTests::RunAllUpscaleInTimeTests()
    {
        UpscaleInTimeTests1();
        UpscaleInTimeTests2();
        UpscaleInTimeTests3();
        UpscaleInTimeTests4();
        UpscaleInTimeTests5();
        UpscaleInTimeTests6();
        UpscaleInTimeTests7();
        UpscaleInTimeTests8();
    }

    void UpscaleTests::RunAllEquivalentAlphaTests()
    {
        EquivalentAlphaTesting1();
        EquivalentAlphaTesting2();
        EquivalentAlphaTesting3();
        EquivalentAlphaTesting4();
        EquivalentAlphaTesting5();
        EquivalentAlphaTesting6();
    }

    void UpscaleTests::RunAllUpscaleToLargestBlockTests()
    {
        UpscaleToLargestBlockTests1();
        UpscaleToLargestBlockTests2();
        UpscaleToLargestBlockTests3();
    }

    // Testing the upscaling of probabilities in time.
    // Test with 10 fully correlated elements.
    // Beta=3.5, and the design point is equal to (1 1 1 1 1)/Sqrt(5)
    // The resulting beta and alfa's should be equal to the values for the single element
    void UpscaleTests::UpscaleInTimeTests1()
    {
        constexpr size_t nr_stochasts = 5;
        constexpr int nr_elements = 10;
        auto element = alphaBeta(3.5, vector1D(nr_stochasts));     //equal alpha values
        element.assign(1.0);
        element.normalize();
        auto in_rho_t = vector1D(nr_stochasts);
        in_rho_t.assign(1.0); // all rho values set to one

        const auto elementORG = element;  // Copy of the original beta and alpha values

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);

        test_utilities.checkAlphaBeta(element, elementORG, 0.001);
    }

    // Testing the upscaling of probabilities in time.
    // Test with 10 uncorrelated elements.
    // Beta=3.5, and the design point is equal to (1 0 0 0 0)
    // The resulting beta is verified and alfa's should be equal to the values for the single element
    void UpscaleTests::UpscaleInTimeTests2()
    {
        constexpr size_t nr_stochasts = 5;
        constexpr int nr_elements = 10;
        auto element = alphaBeta(3.5, { 1.0, 0.0, 0.0, 0.0, 0.0 });
        const auto in_rho_t = vector1D(nr_stochasts); // defaults to 0.0

        const auto p = element.getP();
        const auto beta = Statistics::StandardNormal::getUFromQ(1.0 - pow(p, nr_elements));
        const auto ref = alphaBeta(beta, element.getAlpha());

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);

        test_utilities.checkAlphaBeta(element, ref, 0.001);
    }

    // Testing the upscaling of probabilities in time.
    // Test with 10 uncorrelated elements.
    // Beta=3.5, and the design point is equal to (0 0.6 0 0.8 0)
    // The resulting beta is verified and alfa's should be equal to the values for the single element
    void UpscaleTests::UpscaleInTimeTests3()
    {
        constexpr size_t nr_stochasts = 5;
        constexpr int nr_elements = 10;
        auto element = alphaBeta(3.5, { 0.0, 0.6, 0.0, 0.8, 0.0 });
        const auto in_rho_t = vector1D(nr_stochasts);

        const auto p = element.getP();
        const auto beta = Statistics::StandardNormal::getUFromQ(1.0 - pow(p, nr_elements));
        const auto ref = alphaBeta(beta, element.getAlpha());

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);

        test_utilities.checkAlphaBeta(element, ref, 0.001);
    }

    // Testing the upscaling of probabilities in time.
    // Test with 10 uncorrelated elements.
    // Beta=3.5, and the design point is equal to (-1 0 0 0 0)
    // The resulting beta is verified and alfa's should be equal to the values for the single element
    void UpscaleTests::UpscaleInTimeTests4()
    {
        constexpr size_t nr_stochasts = 5;
        constexpr int nr_elements = 10;
        auto element = alphaBeta(3.5, { -1.0, 0.0, 0.0, 0.0, 0.0 });
        const auto in_rho_t = vector1D(nr_stochasts);

        const auto p = element.getP();
        const auto beta = Statistics::StandardNormal::getUFromQ(1.0 - pow(p, nr_elements));
        const auto ref = alphaBeta(beta, element.getAlpha());

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);

        test_utilities.checkAlphaBeta(element, ref, 0.001);
    }

    // Testing the upscaling of probabilities in time.
    // Test with 10 uncorrelated elements.
    // Beta=3.5, and the design point is equal to (0 0.6 0 -0.8 0)
    // The resulting beta is verified and alfa's should be equal to the values for the single element
    void UpscaleTests::UpscaleInTimeTests5()
    {
        constexpr size_t nr_stochasts = 5;
        constexpr int nr_elements = 10;
        auto element = alphaBeta(3.5, { 0.0, 0.6, 0.0, -0.8, 0.0 });
        const auto in_rho_t = vector1D(nr_stochasts);

        const auto p = element.getP();
        const auto beta = Statistics::StandardNormal::getUFromQ(1.0 - pow(p, nr_elements));
        const auto ref = alphaBeta(beta, element.getAlpha());

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);

        test_utilities.checkAlphaBeta(element, ref, 0.001);
    }

    // Testing the upscaling of probabilities in time.
    // Test with 10 almost fully correlated elements.
    // Beta= 3.5, and the design point is equal to (0, 0.6, 0, -0.8, 0)
    // The resulting beta is verified and alfa's should be equal to the values for the single element
    void UpscaleTests::UpscaleInTimeTests6()
    {
        constexpr size_t nr_stochasts = 5;
        constexpr int nr_elements = 10;
        auto element = alphaBeta(3.5, { 0.0, 0.6, 0.0, -0.8, 0.0 });
        auto in_rho_t = vector1D(nr_stochasts);
        in_rho_t.assign(0.999999);

        const auto ref = alphaBeta(3.49846239808633, element.getAlpha());

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);

        test_utilities.checkAlphaBeta(element, ref, 1.0e-6);
    }

    // case from issue HRING-1349
    // two almost identical situations before upscaling in time.
    // however, the resulting alpha(12) differs largely
    void UpscaleTests::UpscaleInTimeTests7()
    {
        constexpr double margin = 1.0e-6; // acceptable margin for difference between beta original and beta computed
        constexpr size_t nr_stochasts = 12;      // Number of stochastic variables
        constexpr double nr_elements = 365.25; // Number of time elements (e.g. tidal periods)

        // case 1:
        auto element = alphaBeta(11.57868694964,
            { -3.161166262316398E-6, 0.151673576904618, -8.746544990273167E-2,
             -8.746544990273167E-2, -0.156371455735438,    -0.912570913068033,
              0.250863571511060,     3.568922013287609E-4,  0.203773500205695,
              3.810363930428925E-4,  1.919024503886323E-4, -7.296398126214022E-4 });
        auto in_rho_t = vector1D(nr_stochasts);
        in_rho_t.assign(1.0);
        in_rho_t(nr_stochasts - 1) = 0.0;

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);

        ASSERT_NEAR(element.getBeta(), 11.5765466697374, margin);
        ASSERT_NEAR(element.getAlphaI(nr_stochasts - 1), -0.311985E-3, margin);

        // case 2:
        // relative differences with case 1 approx. 1E-4
        element = alphaBeta(11.5805537878769,
            { -3.161065009975362E-6,  0.151647352510170,    -8.745032983732001E-2,
             -8.745032983732001E-2, -0.156344425365967,    -0.912591229826987,
              0.250861020035668,     3.568801510567352E-4,  0.203738889137470,
              3.809716860474264E-4,  1.918698629447029E-4, -7.296245519395209E-4 });

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);

        ASSERT_NEAR(element.getBeta(), 11.5783936419198, margin);
        ASSERT_NEAR(element.getAlphaI(nr_stochasts - 1), -0.27259E-3, margin);
    }

    // test upscaling with nrElements = 1.0
    // this should give the same alpha and beta as the input values.
    // however, the alpha values differ ~ 1e-10
    void UpscaleTests::UpscaleInTimeTests8()
    {
        constexpr double margin_beta = 1.0e-13; // acceptable margin for difference between beta original and beta computed
        constexpr double margin_alpha = 1.0e-10; // acceptable margin for difference between alpha original and alpha computed
        constexpr size_t nr_stochasts = 2;       // Number of stochastic variables
        constexpr double nr_elements = 1.0; // Number of time elements (e.g. tidal periods)

        auto element = alphaBeta(4.8, { 1.0, 0.001 });
        element.normalize();
        auto in_rho_t = vector1D(nr_stochasts);
        in_rho_t.assign(1.0);
        in_rho_t(nr_stochasts - 1) = 0.0;
        auto alpha_ref = element.getAlpha();

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);

        ASSERT_NEAR(element.getBeta(), 4.8, margin_beta);
        ASSERT_NEAR(element.getAlphaI(0), alpha_ref(0), margin_alpha);
        ASSERT_NEAR(element.getAlphaI(1), alpha_ref(1), margin_alpha);
    }

    // Testing of the consistency of the equivalent alpha according to the methods
    // upscaleInTime and combineTwoElementsPartialCorrelation
    // Test with 2 partially correlated elements.
    // Beta is 3.5, the design point is equal to (0.6 0.8), and Rho is equal to (0.5 0.5)
    // The resulting beta is verified and resulting alfa's should be (approximately) equal
    void UpscaleTests::EquivalentAlphaTesting1()
    {
        //  Initialisations:
        constexpr int nr_elements = 2;

        auto element = alphaBeta(3.5, { 0.6, 0.8 });
        auto in_rho_t = vector1D({ 0.5, 0.5 });

        auto original_element = element;   // copy of original beta

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);
        auto element_3 = combiner.combineTwoElementsPartialCorrelation(original_element, original_element, in_rho_t, combineAndOr::combOr);

        test_utilities.checkAlphaBeta(element, element_3.ab, 1.0e-3);
        EXPECT_EQ(element_3.n, 0);
    }

    // Testing of the consistency of the equivalent alpha according to the methods upscaleInTime and combineTwoElementsPartialCorrelation
    // Test with 2 partially correlated elements.
    // Beta is 3.5, the design point is equal to (0.6 0.8), and Rho is equal to (1.0 0.5)
    // The resulting beta is verified and resulting alfa's should be (approximately) equal
    void UpscaleTests::EquivalentAlphaTesting2()
    {
        constexpr double nr_elements = 2.0;
        auto element = alphaBeta(3.5, { 0.6, 0.8 });
        const auto in_rho_t = vector1D({ 1.0, 0.5 });

        const auto original_element = element;

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);
        auto C = combiner.combineTwoElementsPartialCorrelation(original_element, original_element, in_rho_t, combineAndOr::combOr);

        EXPECT_NEAR(C.ab.getBeta(), element.getBeta(), 2e-3);
        EXPECT_EQ(C.n, 0);
        // no check on alpha values in Fortran code
    }

    // Testing of the consistency of the equivalent alpha according to the methods upscaleInTime and combineTwoElementsPartialCorrelation
    // Test with 2 fully correlated elements.
    // Beta is 3.5, the design point is equal to (1.0 0.0), and Rho is equal to (1.0 0.0)
    // The resulting beta is verified and resulting alfa's should be (approximately) equal
    void UpscaleTests::EquivalentAlphaTesting3()
    {
        constexpr double nr_elements = 2.0;

        auto elm = alphaBeta(3.5, { 1.0, 0.0 });
        const auto in_rho_t = vector1D({ 1.0, 0.0 });

        const auto original_element = elm; // copy of original elm

        upscaler.upscaleInTime(nr_elements, elm, in_rho_t);
        auto C = combiner.combineTwoElementsPartialCorrelation(original_element, original_element, in_rho_t, combineAndOr::combOr);

        test_utilities.checkAlphaBeta(elm, C.ab, 1e-3);
        EXPECT_EQ(C.n, 0);
    }

    // Testing of the consistency of the equivalent alpha according to the methods upscaleInTime and combineTwoElementsPartialCorrelation
    // Test with 2 uncorrelated elements.
    // Beta is 3.5, the design point is equal to (1.0 0.0), and Rho is equal to (0.0 1.0)
    // The resulting beta is verified and resulting alfa's should be (approximately) equal
    void UpscaleTests::EquivalentAlphaTesting4()
    {
        constexpr double nr_elements = 2.0;

        auto elm = alphaBeta(3.5, { 1.0, 0.0 });
        auto inRhoT = vector1D({ 0.0, 1.0 });

        auto ORG = elm; // copy of original elm

        upscaler.upscaleInTime(nr_elements, elm, inRhoT);
        auto C = combiner.combineTwoElementsPartialCorrelation(ORG, ORG, inRhoT, combineAndOr::combOr);

        test_utilities.checkAlphaBeta(elm, C.ab, 1e-3);
        EXPECT_EQ(C.n, 0);
    }

    // Testing of the consistency of the equivalent alpha according to the methods upscaleInTime and combineTwoElementsPartialCorrelation \n
    // Test with 2 partially correlated elements. \n
    // Beta is 3.5, the design point is equal to (0.6 -0.8), and Rho is equal to (1.0 0.5)
    // The resulting beta is verified and resulting alfa's should be (approximately) equal
    void UpscaleTests::EquivalentAlphaTesting5()
    {
        constexpr double nr_elements = 2.0;

        auto element = alphaBeta(3.5, { 0.6, -0.8 });
        const auto in_rho_t = vector1D({ 1.0, 0.5 });

        const auto original_element = element; // copy of original elm

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);
        const auto C = combiner.combineTwoElementsPartialCorrelation(original_element, original_element, in_rho_t, combineAndOr::combOr);

        EXPECT_NEAR(C.ab.getBeta(), element.getBeta(), 2e-3);
        EXPECT_EQ(C.n, 0);
        // no check on alpha values in Fortran code
    }

    // Testing of the consistency of the equivalent alpha according to the methods upscaleInTime and combineTwoElementsPartialCorrelation
    // Test with 2 partially correlated elements.
    // Beta is 3.5, the design point is equal to (0.6 -0.8), and Rho is equal to (1.0 0.0)
    // The resulting beta is verified and resulting alfa's should be (approximately) equal
    void UpscaleTests::EquivalentAlphaTesting6()
    {
        constexpr double nr_elements = 2.0;

        auto element = alphaBeta(3.5, { 0.6, -0.8 });
        const auto in_rho_t = vector1D({ 1.0, 0.0 });

        const auto original_element = element; // copy of original element

        upscaler.upscaleInTime(nr_elements, element, in_rho_t);
        auto C = combiner.combineTwoElementsPartialCorrelation(original_element, original_element, in_rho_t, combineAndOr::combOr);
        test_utilities.checkAlphaBeta(element, C.ab, 1e-3, 2e-2);
        EXPECT_EQ(C.n, 0);
    }

    // Testing the upscaling of probabilities to the largest block duration.
    // Test with 5 random variables, beta=3.5, alpha is random and block durations are equal.
    // The resulting beta and alfa's should be equal to the values for the single element
    void UpscaleTests::UpscaleToLargestBlockTests1()
    {
        constexpr size_t nr_stochasts = 5;
        constexpr double beta = 3.5;
        auto alpha = vector1D({ 1.0, 2.0, 3.0, 4.0, 5.0 }); // alpha values
        alpha.normalize();
        auto duration = vector1D(nr_stochasts);
        duration.assign(1.0);                        // individual block durations
        const auto in_rho_t = vector1D({ 0.1, 0.3, 0.5, 0.7, 0.9 }); // all rho values

        const auto original_design_point = alphaBeta(beta, alpha); // Copy double of the original beta value
        const auto duration_org = duration;  // Copy of the original durations
        const double max_duration = duration.maxval();

        alphaBeta elm;
        upscaler.upscaleToLargestBlock(original_design_point, in_rho_t, duration_org, max_duration, elm, duration);

        test_utilities.checkAlphaBeta(elm, original_design_point, 0.001);
    }

    // Testing the upscaling of probabilities to the largest block duration.
    // Test with 5 random variables, beta=3.5, alpha is random and block durations are equal.
    // The resulting beta and alfa's should be equal to the values for the single element
    void UpscaleTests::UpscaleToLargestBlockTests2()
    {
        constexpr size_t nr_stochasts = 5;
        constexpr double beta = 3.5;
        auto alpha = vector1D(nr_stochasts); // equal alpha values
        alpha.assign(1.0); alpha.normalize();
        auto duration = vector1D({ 1.0, 2.0, 3.0, 4.0, 5.0 }); // individual block durations
        auto in_rho_t = vector1D(nr_stochasts); // all rho values
        in_rho_t.assign(1.0);

        const auto original_design_point = alphaBeta(beta, alpha); // Copy of the original beta and alpha values
        const auto duration_org = duration; // Copy of the original durations
        const auto max_duration = duration.maxval();

        alphaBeta elm;
        upscaler.upscaleToLargestBlock(original_design_point, in_rho_t, duration_org, max_duration, elm, duration);

        test_utilities.checkAlphaBeta(elm, original_design_point, 0.001);
    }

    // Testing the upscaling of probabilities to the largest block duration.
    // Test with 5 random variables, beta=3.5, design point is equal to (0 0.6 0 0.8 0)
    // Auto-correlations are all set to zero. Block durations of variables with alpha>0 differ by a factor 10 with the maximum.
    // The resulting beta is verified (see upscaleInTimeTest3) and alfa's should be equal to the values for the single element.
    void UpscaleTests::UpscaleToLargestBlockTests3()
    {
        constexpr size_t nr_stochasts = 5;
        constexpr double beta = 3.5;
        const auto alpha = vector1D({ 0.0, 0.6, 0.0, 0.8, 0.0 });     // alpha values
        auto duration = vector1D({ 20.0, 2.0, 1.0, 2.0, 10.0 });   // individual block durations
        const auto in_rho_t = vector1D(nr_stochasts);               // all rho values set to zero

        const auto original_design_point = alphaBeta(beta, alpha); // Copy of the original beta and alpha values
        const auto duration_org = duration;  // Copy of the original durations
        const auto max_duration = duration.maxval();

        const double p = Statistics::StandardNormal::getPFromU(beta);
        const auto beta2 = Statistics::StandardNormal::getUFromQ(1.0 - pow(p, 10));
        const auto ref = alphaBeta(beta2, original_design_point.getAlpha());

        alphaBeta elm;
        upscaler.upscaleToLargestBlock(original_design_point, in_rho_t, duration_org, max_duration, elm, duration);

        test_utilities.checkAlphaBeta(elm, ref, 0.001);
    }

}

