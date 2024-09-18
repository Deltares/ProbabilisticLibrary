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
#pragma once
#include "../Utils/testutils.h"
#include "../../Deltares.Probabilistic/Combine/combineElements.h"
#include "../../Deltares.Probabilistic/Combine/upscaling.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class combinElementsTests
            {
            public:
                void runAll();
            private:
                void testCombineTwoElementsNoCorrelation1();
                void testCombineTwoElementsNoCorrelation2();
                void testCombineTwoElementsNoCorrelation3();
                void testCombineTwoElementsNoCorrelation4();
                void testCombineTwoElementsNoCorrelation5();
                void testCombineTwoElementsNoCorrelation6();
                void testcombineTwoElementsPartialCorrelation1();
                void testcombineTwoElementsPartialCorrelation2();
                void testcombineTwoElementsPartialCorrelation3();
                void testcombineTwoElementsPartialCorrelation4();
                void testcombineTwoElementsPartialCorrelation5();
                void testcombineTwoElementsPartialCorrelation6();
                void testcombineTwoElementsPartialCorrelation7();
                void testcombineTwoElementsPartialCorrelation8();
                void testcombineTwoElementsPartialCorrelation9();
                void testcombineTwoElementsPartialCorrelation10();
                void testcombineTwoElementsPartialCorrelation11();
                void testcombineTwoElementsPartialCorrelation12();
                void testcombineTwoElementsPartialCorrelation13();
                void testcombineTwoElementsPartialCorrelation14();
                void testcombineTwoElementsPartialCorrelation15();
                void testcombineTwoElementsPartialCorrelation99();
                void testcombineTwoElementsPCorWithNonContrAlphas();
                void testcombineMultipleElementsProb1();
                void testcombineMultipleElementsProb2();
                void testcombineMultipleElementsProb3();
                void testcombineMultipleElementsProb4();
                void testcombineMultipleElementsProb5();
                void testcombineMultipleElementsSpatialCorrelated1();
                void testcombineMultipleElementsSpatialCorrelated2();
                void testcombineMultipleElementsSpatialCorrelated3();
                void testCombineElementsFullCorrelation1();
                void testCombineElementsFullCorrelation2();
                void testCombineElementsFullCorrelation(const Deltares::Reliability::combineAndOr andOr);
                void testcombineTwoElementsNegativeCorrelation1();
                void testcombineTwoElementsNegativeCorrelation2();
                void testcombineThreeElementsPartialCorrelation1();
                void testcombineThreeElementsPartialCorrelation2();
                void testcombineThreeElementsPartialCorrelation3();
                void testcombineThreeElementsPartialCorrelation4();
                void testcombineTwoElementsPartialCorrelationPiping();
                void testcombineTwoElementsPartialCorrelation9a();
                void testcombineTwoElementsPartialCorrelation10a();
                void testcombineTwoElementsPartialCorrelation11a();
                void testcombineTwoElementsPartialCorrelation12a();
                void testcombineTwoElementsPartialCorrelation13a();
                void testcombineTwoElementsPartialCorrelation14a();
                void testcombineTwoElementsPartialCorrelation99a();
                Deltares::Reliability::combineElements cmb;
                Deltares::Reliability::upscaling up;
                testutils utils;
            };
        }
    }
}
