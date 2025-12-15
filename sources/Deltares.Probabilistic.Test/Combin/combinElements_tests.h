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
#include "../Utils/testutils.h"
#include "../../Deltares.Probabilistic/Combine/combineElements.h"
#include "../../Deltares.Probabilistic/Combine/upscaling.h"

namespace Deltares::Probabilistic::Test
{
    class combinElementsTests
    {
    public:
        void runAllCombineTwoElementsTests();
        void runAllLengthEffectTests();
    private:
        void testCombineTwoElementsNoCorrelation1();
        void testCombineTwoElementsNoCorrelation2();
        void testCombineTwoElementsNoCorrelation3();
        void testCombineTwoElementsNoCorrelation4();
        void testCombineTwoElementsNoCorrelation5();
        void testCombineTwoElementsNoCorrelation6();
        void testCombineTwoElementsPartialCorrelation1();
        void testCombineTwoElementsPartialCorrelation2();
        void testCombineTwoElementsPartialCorrelation3();
        void testCombineTwoElementsPartialCorrelation4();
        void testCombineTwoElementsPartialCorrelation5();
        void testCombineTwoElementsPartialCorrelation6();
        void testCombineTwoElementsPartialCorrelation7();
        void testCombineTwoElementsPartialCorrelation8();
        void testCombineTwoElementsPartialCorrelation9();
        void testCombineTwoElementsPartialCorrelation10();
        void testCombineTwoElementsPartialCorrelation11();
        void testCombineTwoElementsPartialCorrelation12();
        void testCombineTwoElementsPartialCorrelation13();
        void testCombineTwoElementsPartialCorrelation14();
        void testCombineTwoElementsPartialCorrelation15();
        void testCombineTwoElementsPartialCorrelation99();
        void testCombineTwoElementsPCorWithNonContrAlphas();
        void testCombineMultipleElementsProb1();
        void testCombineMultipleElementsProb2();
        void testCombineMultipleElementsProb3();
        void testCombineMultipleElementsProb4();
        void testCombineMultipleElementsProb5();
        void testCombineMultipleElementsSpatialCorrelated1();
        void testCombineMultipleElementsSpatialCorrelated2();
        void testCombineMultipleElementsSpatialCorrelated3();
        void testLengthEffectFourStochasts();
        void testCombineElementsFullCorrelation1();
        void testCombineElementsFullCorrelation2();
        void testCombineElementsFullCorrelation(const Reliability::combineAndOr andOr);
        void testCombineTwoElementsNegativeCorrelation1();
        void testCombineTwoElementsNegativeCorrelation2();
        void testCombineThreeElementsPartialCorrelation1();
        void testCombineThreeElementsPartialCorrelation2();
        void testCombineThreeElementsPartialCorrelation3();
        void testCombineThreeElementsPartialCorrelation4();
        void testCombineTwoElementsPartialCorrelationPiping();
        void TestCombineTwoElementsPartialCorrelationRealCaseA();
        void TestCombineTwoElementsPartialCorrelationRealCaseB();
        void TestCombineTwoElementsPartialCorrelationRealCaseC();
        void testCombineTwoElementsPartialCorrelation9a();
        void testCombineTwoElementsPartialCorrelation10a();
        void testCombineTwoElementsPartialCorrelation11a();
        void testCombineTwoElementsPartialCorrelation12a();
        void testCombineTwoElementsPartialCorrelation13a();
        void testCombineTwoElementsPartialCorrelation14a();
        void testCombineTwoElementsPartialCorrelation99a();
        Reliability::combineElements combiner;
        Reliability::upscaling upscaler;
        testutils test_utilities;
    };
}

