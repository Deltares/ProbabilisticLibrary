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
#include "Math/testRootfinder.h"
#include "Combin/hohenbichler_tests.h"
#include "Combin/intEqualElements_tests.h"
#include "Combin/upscale_tests.h"
#include "Combin/combinElements_tests.h"
#include "Combin/combiner_tests.h"
#include "Statistics/testStandardNormal.h"
#include "Distributions/testDistributions.h"
#include "Model/TestRunModel.h"
#include "Uncertainty/TestUncertainty.h"
#include "Sensitivity/TestSensitivity.h"
#include "Optimization/testCobyla.h"
#include "Reliability/testWaartsLinearResistance.h"
#include "Reliability/testWaartsNoisyLimitState.h"
#include "Reliability/testWaartsResistanceOneQuadratic.h"

using namespace Deltares::Probabilistic::Test;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "reliability_method_test.testClustersAdpImpSampling";
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
    testNumericSupport::allNumericSupportTests();
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

TEST(reliability_method_test, testFORM)
{
    auto tstRelMethods = testReliabilityMethods();
    tstRelMethods.testFORM();
}

TEST(reliability_method_test, testFORMArray)
{
    auto tstRelMethods = testReliabilityMethods();
    tstRelMethods.testFORMArray();
}

TEST(reliability_method_test, testFORMVaryingArray)
{
    auto tstRelMethods = testReliabilityMethods();
    tstRelMethods.testFORMVaryingArray();
}

TEST(reliability_method_test, testValidation)
{
    testReliabilityMethods::testValidation();
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

TEST(reliability_method_test, testSubSetSimulationReliabilityNearestToMean)
{
    testReliabilityMethods::testSubSetSimulationReliabilityNearestToMean();
}

TEST(reliability_method_test, testSubSetSimulationReliabilityCenterOfGravity)
{
    testReliabilityMethods::testSubSetSimulationReliabilityCenterOfGravity();
}

TEST(reliability_method_test, testFDIRReliability)
{
    testReliabilityMethods::testFDIRReliability();
}

TEST(reliability_method_test, testDSFIReliability)
{
    testReliabilityMethods::testDSFIReliability();
}

TEST(reliability_method_test, testFragilityCurveIntegration)
{
    testReliabilityMethods::testFragilityCurveIntegration();
}

TEST(reliability_method_test, testNumericalIntegrationReliability)
{
    testReliabilityMethods::testNumericalIntegrationReliability();
}

TEST(reliability_method_test, testCrudeMCReliability)
{
    testReliabilityMethods::testCrudeMonteCarloReliability();
}

TEST(reliability_method_test, testAdaptiveImportanceSampling)
{
    testReliabilityMethods::testAdaptiveImportanceSampling();
}

TEST(reliability_method_test, testClustersAdpImpSampling)
{
    testReliabilityMethods::testClustersAdpImpSampling();
}

TEST(reliability_method_test, testDirSamplingTwoBranches)
{
    testReliabilityMethods::testDirSamplingProxyModels(false, ModelVaryingType::Monotone, 1.0);
}

TEST(reliability_method_test, testDirSamplingProxyModels)
{
    testReliabilityMethods::testDirSamplingProxyModels(true, ModelVaryingType::Monotone, 1.0);
}

TEST(reliability_method_test, testDirSamplingTwoBranchesNotMonotone)
{
    testReliabilityMethods::testDirSamplingProxyModels(false, ModelVaryingType::Varying, 1.0);
}

TEST(reliability_method_test, testDirSamplingProxyModelsNotMonotone)
{
    testReliabilityMethods::testDirSamplingProxyModels(true, ModelVaryingType::Varying, 1.0);
}

TEST(reliability_method_test, testDirSamplingTwoBranchesLargerStepsize)
{
    testReliabilityMethods::testDirSamplingProxyModels(false, ModelVaryingType::Monotone, 3.0);
}
TEST(distribution_tests, testConditionalStochast)
{
    testDistributions::testConditionalStochast();
}

TEST(distribution_tests, testComposite)
{
    testDistributions::testComposite();
}

TEST(distribution_tests, testCompositeConditionalStochast)
{
    testDistributions::testCompositeConditionalStochast();
}

TEST(distribution_tests, testCompositeConditionalTruncatedStochast)
{
    testDistributions::testCompositeConditionalTruncatedStochast();
}

TEST(distribution_tests, testCompositeStochast)
{
    testDistributions::testCompositeStochast();
}

TEST(distribution_tests, testCompositeTruncatedStochast)
{
    testDistributions::testCompositeTruncatedStochast();
}

TEST(distribution_tests, testConditionalWeibull)
{
    testDistributions::testConditionalWeibull();
}

TEST(distribution_tests, testConditionalWeibullCdfPdf)
{
    testDistributions::testConditionalWeibullCdfPdf();
}

TEST(distribution_tests, testConditionalWeibullCdfPdf2)
{
    testDistributions::testConditionalWeibullCdfPdf2();
}

TEST(distribution_tests, testConditionalWeibullMeanDeviation)
{
    testDistributions::testConditionalWeibullMeanDeviation();
}

TEST(distribution_tests, testConditionalWeibullNonIntegerShape)
{
    testDistributions::testConditionalWeibullNonIntegerShape();
}

TEST(distribution_tests, testDesignValue)
{
    testDistributions::testDesignValue();
}

TEST(distribution_tests, testNormal)
{
    testDistributions::testNormal();
}

TEST(distribution_tests, testLogNormal)
{
    testDistributions::testLogNormal();
}

TEST(distribution_tests, testUniform)
{
    testDistributions::testUniform();
}

TEST(distribution_tests, testTriangular)
{
    testDistributions::testTriangular();
}

TEST(distribution_tests, testTrapezoidal)
{
    testDistributions::testTrapezoidal();
}

TEST(distribution_tests, testExponential)
{
    testDistributions::testExponential();
}

TEST(distribution_tests, testGamma)
{
    testDistributions::testGamma();
}

TEST(distribution_tests, testBeta)
{
    testDistributions::testBeta();
}

TEST(distribution_tests, testGumbel)
{
    testDistributions::testGumbel();
}

TEST(distribution_tests, testWeibull)
{
    testDistributions::testWeibull();
}

TEST(distribution_tests, testFrechet)
{
    testDistributions::testFrechet();
}

TEST(distribution_tests, testGEV)
{
    testDistributions::testGEV();
}

TEST(distribution_tests, testPareto)
{
    testDistributions::testPareto();
}

TEST(distribution_tests, testGeneralizedPareto)
{
    testDistributions::testGeneralizedPareto();
}

TEST(distribution_tests, testRayleigh)
{
    testDistributions::testRayleigh();
}

TEST(distribution_tests, testRayleighN)
{
    testDistributions::testRayleighN();
}

TEST(distribution_tests, testBernoulli)
{
    testDistributions::testBernoulli();
}

TEST(distribution_tests, testPoisson)
{
    testDistributions::testPoisson();
}

TEST(distribution_tests, testStudentT)
{
    testDistributions::testStudentT();
}

TEST(distribution_tests, testStudentTwithInterpolation)
{
    testDistributions::testStudentTwithInterpolation();
}

TEST(distribution_tests, testValidation)
{
    testDistributions::testValidation();
}

TEST(distribution_tests, testVariationCoefficient)
{
    testDistributions::testVariationCoefficient();
}

TEST(unittst, testStandardNormal)
{
    auto tstStdNormal = testStandardNormal();
    tstStdNormal.allStandardNormalTests();
}

TEST(unittst, testUncertainty)
{
    auto tstUncertainty = TestUncertainty();
    tstUncertainty.allUncertaintyTests();
}

TEST(unittst, testSensitivity)
{
    auto tstSensitivity = TestSensitivity();
    tstSensitivity.allSensitivityTests();
}

TEST(unittst, testRunModel)
{
    auto tstModel = TestRunModel();
    tstModel.allModelTests();
}

TEST(unittst, testRandom)
{
    auto tstRnd = testRandom();
    tstRnd.allRandomTests();
}

TEST(unittst, testRootfinder)
{
    auto tstRF = rootfinder_tests();
    tstRF.all_rootfinder_tests();
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

TEST(WaartsTests, LinearResistanceFORM)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsFORM();
}

TEST(WaartsTests, LinearResistanceCrudeMonteCarlo)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, LinearResistanceDirectionalSampling)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, LinearResistanceNumericalIntegration)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, LinearResistanceImportanceSampling)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, LinearResistanceAdaptiveImportanceSampling)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, LinearResistanceFDIR)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsFDIR();
}

TEST(WaartsTests, LinearResistanceDSFI)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsDSFI();
}

TEST(WaartsTests, NoisyLimitStateFORM)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsFORM();
}

TEST(WaartsTests, NoisyLimitStateCrudeMonteCarlo)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, NoisyLimitStateDirectionalSampling)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, NoisyLimitStateImportanceSampling)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, NoisyLimitStateAdaptiveImportanceSampling)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, NoisyLimitStateFDIR)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsFDIR();
}

TEST(WaartsTests, NoisyLimitStateDSFI)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsDSFI();
}

TEST(WaartsTests, ResistanceOneQuadraticFORM)
{
    auto tester = TestWaartsResistanceOneQuadratic();
    tester.WaartsFORM();
}

TEST(WaartsTests, ResistanceOneQuadraticCrudeMonteCarlo)
{
    auto tester = TestWaartsResistanceOneQuadratic();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, ResistanceOneQuadraticDirectionalSampling)
{
    auto tester = TestWaartsResistanceOneQuadratic();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, ResistanceOneQuadraticNumericalIntegration)
{
    auto tester = TestWaartsResistanceOneQuadratic();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, ResistanceOneQuadraticImportanceSampling)
{
    auto tester = TestWaartsResistanceOneQuadratic();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, ResistanceOneQuadraticAdaptiveImportanceSampling)
{
    auto tester = TestWaartsResistanceOneQuadratic();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, ResistanceOneQuadraticFDIR)
{
    auto tester = TestWaartsResistanceOneQuadratic();
    tester.WaartsFDIR();
}

TEST(WaartsTests, ResistanceOneQuadraticDSFI)
{
    auto tester = TestWaartsResistanceOneQuadratic();
    tester.WaartsDSFI();
}

