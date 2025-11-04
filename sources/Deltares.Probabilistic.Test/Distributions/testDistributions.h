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
    private:
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
        static void testPoisson();
        static void testGamma();
        static void testStudentT();
        static void testStudentTwithInterpolation();
        static void testComposite();
        static void testFitNormal();
        static void testFitLogNormal();
        static void testFitBernoulli();
        static void testValidation();
        static double getPdfNumerical(Statistics::Stochast& s, const double x);
    };
}

