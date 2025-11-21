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
#pragma once
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares::Probabilistic::Test
{
    class testDistributions
    {
    public:
        static void allDistributionsTests();
        static void testConditionalWeibull();
        static void testConditionalWeibullNonIntegerShape();
        static void testConditionalWeibullMeanDeviation();
        static void testConditionalWeibullCdfPdf();
        static void testConditionalWeibullCdfPdf2();
        static void testConditionalStochast();
        static void testCompositeStochast();
        static void testCompositeTruncatedStochast();
        static void testCompositeConditionalStochast();
        static void testCompositeConditionalTruncatedStochast();
        static void testDesignValue();
        static void testVariationCoefficient();
        static void testNormal();
        static void testLogNormal();
        static void testUniform();
        static void testTriangular();
        static void testTrapezoidal();
        static void testExponential();
        static void testGumbel();
        static void testFrechet();
        static void testWeibull();
        static void testGEV();
        static void testPareto();
        static void testGeneralizedPareto();
        static void testRayleigh();
        static void testRayleighN();
        static void testBernoulli();
        static void testPoisson();
        static void testGamma();
        static void testBeta();
        static void testStudentT();
        static void testStudentTwithInterpolation();
        static void testStudentTwithInterpolationLargeNoObservations();
        static void testComposite();
        static void testValidation();
    private:
        static double getPdfNumerical(Statistics::Stochast& s, const double x);
        static void testFit(Statistics::Stochast& stochast, const double margin = 0.1, const int number = 1000);
        static void testInvert(Statistics::Stochast& stochast, const double margin = 0.01);
    };
}

