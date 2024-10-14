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
#include <gtest/gtest.h>
#include "Reliability/testStartPointCalculator.h"
#include "Reliability/testReliabilityMethods.h"
#include "Math/testCholeskiDecomposition.h"
#include "Math/testNumericSupport.h"
#include "Math/testKMean.h"
#include "Math/testMatrixInverse.h"
#include "Math/testMatrixMultiplication.h"
#include "Math/testRandom.h"
#include "Math/testVector1D.h"
#include "Combin/hohenbichler_tests.h"
#include "Combin/intEqualElements_tests.h"
#include "Combin/upscale_tests.h"
#include "Combin/combinElements_tests.h"
#include "Combin/combiner_tests.h"
#include "Statistics/testStandardNormal.h"
#include "Distributions/testDistributions.h"
#include "Sensitivity/TestSensitivity.h"
#include "Optimization/testCobyla.h"

using namespace Deltares::Probabilistic;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            int main(int argc, char** argv)
            {
                ::testing::InitGoogleTest(&argc, argv);
                return RUN_ALL_TESTS();
            }

            TEST(unittst, testCombiner)
            {
                auto h = CombinerTest();
                h.allCombinerTests();
            }

            TEST(unittst, testCombinElements)
            {
                auto h = combinElementsTests();
                h.runAllCombineTwoElementsTests();
            }

            TEST(unittst, testLengthEffect)
            {
                auto h = combinElementsTests();
                h.runAllLengthEffectTests();
            }

            TEST(unittst, testUpscaling)
            {
                auto h = upscale_tests();
                h.runAll();
            }

            TEST(unittst, testIntEqualElements)
            {
                auto h = intEqualElements_tests();
                h.allIntegrateEqualElementsTests();
            }

            TEST(unittst, testHohenbichler)
            {
                auto h = HohenbichlerTest();
                h.allHohenbichlerTests();
            }

            TEST(unittst, testCholeskiDecomp)
            {
                auto tstCholeskiDecomp = choleski_decomp_tests();
                tstCholeskiDecomp.allCholeskyDecompositionTests();
            }

            TEST(unittst, testMatinv)
            {
                auto tstMatInv = matinv_tests();
                tstMatInv.all_matinv_tests();
            }

            TEST(unittst, testMatmul)
            {
                auto tstMatMul = matmul_tests();
                tstMatMul.all_matmul_tests();
            }

            TEST(unittst, testNumericalSupport)
            {
                auto tstNumSupport = testNumericSupport();
                tstNumSupport.allNumericSupportTests();
            }

            TEST(unittst, testKMeanClustering)
            {
                auto h = testKMean();
                h.allKMeanTests();
            }

            TEST(unittst, testStartPointCalculator)
            {
                auto tstStartPoint = testStartPointCalculator();
                tstStartPoint.allStartPointTests();
            }

            TEST(reliability_method_test, testLatinHyperCube)
            {
                auto tstRelMethods = testReliabilityMethods();
                tstRelMethods.testLatinHyperCube();
            }

            TEST(reliability_method_test, testCobylaReliability)
            {
                auto tstRelMethods = testReliabilityMethods();
                tstRelMethods.testCobylaReliability();
            }

            TEST(reliability_method_test, testNumBisection1)
            {
                auto tstRelMethods = testReliabilityMethods();
                tstRelMethods.testNumericalBisection();
            }

            TEST(reliability_method_test, testNumBisection2)
            {
                auto tstRelMethods = testReliabilityMethods();
                tstRelMethods.testNumericalBisectionLinear();
            }

            TEST(unittst, testDistributions)
            {
                auto tstDist = testDistributions();
                tstDist.allDistributionsTests();
            }

            TEST(unittst, testStandardNormal)
            {
                auto tstStdNormal = testStandardNormal();
                tstStdNormal.allStandardNormalTests();
            }

            TEST(unittst, testSensitivity)
            {
                auto tstSensitivity = TestSensitivity();
                tstSensitivity.allSensitivityTests();
            }

            TEST(unittst, testRandom)
            {
                auto tstRnd = testRandom();
                tstRnd.allRandomTests();
            }

            TEST(unittst, testVector1D)
            {
                auto tstVector1D = vector1D_tests();
                tstVector1D.allVector1Dtests();
            }

            TEST(unittst, testCobylaOptimization)
            {
                auto tstCobylaOpt = testCobyla();
                tstCobylaOpt.allCobylaTests();
            }

        }
    }
}

