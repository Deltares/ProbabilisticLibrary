#pragma once
#include "../utils/testutils.h"
#include "../../src/combine/combineElements.h"
#include "../../src/combine/upscaling.h"

namespace Deltares
{
    namespace ProbLibTests
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
            void testgetMultipleElementsHighestBeta();
            void testgetMultipleElementsLowestBeta();
            void testcombineMultipleElementsSpatialCorrelated1();
            void testcombineMultipleElementsSpatialCorrelated2();
            void testcombineMultipleElementsSpatialCorrelated3();
            void testcombineMultipleElementsSpatialCorrelated4();
            void testCombineElementsFullCorrelation1();
            void testCombineElementsFullCorrelation2();
            void testCombineElementsFullCorrelation(const combineAndOr andOr);
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
            combineElements cmb;
            upscaling up;
            testutils utils;
        };
    }
}
