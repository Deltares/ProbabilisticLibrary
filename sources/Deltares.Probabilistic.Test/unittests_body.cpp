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
#include "Reliability/testWaartsResistanceOneQuadraticTerm.h"
#include "Reliability/testWaartsResistanceTenQuadraticTerms.h"
#include "Reliability/testWaartsResistance25QuadraticTerms.h"
#include "Reliability/testWaartsConvexFailureDomain.h"
#include "Reliability/testWaartsOblateSpheroid.h"
#include "Reliability/testWaartsSaddleSurface.h"
#include "Reliability/testWaartsDiscontinuousLimitState.h"
#include "Reliability/testWaartsTwoBranches.h"
#include "Reliability/testWaartsConcaveFailureDomain.h"
#include "Reliability/testWaartsSeriesSystem.h"

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
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsFORM();
}

TEST(WaartsTests, ResistanceOneQuadraticCrudeMonteCarlo)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, ResistanceOneQuadraticDirectionalSampling)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, ResistanceOneQuadraticNumericalIntegration)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, ResistanceOneQuadraticImportanceSampling)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, ResistanceOneQuadraticAdaptiveImportanceSampling)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, ResistanceOneQuadraticFDIR)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsFDIR();
}

TEST(WaartsTests, ResistanceOneQuadraticDSFI)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsDSFI();
}

TEST(WaartsTests, ResistanceTenQuadraticTermsFORM)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsFORM();
}

TEST(WaartsTests, ResistanceTenQuadraticTermsCrudeMonteCarlo)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, ResistanceTenQuadraticTermsDirectionalSampling)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, ResistanceTenQuadraticTermsImportanceSampling)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, ResistanceTenQuadraticTermsAdaptiveImportanceSampling)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, ResistanceTenQuadraticTermsFDIR)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsFDIR();
}

TEST(WaartsTests, ResistanceTenQuadraticTermsDSFI)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsDSFI();
}

TEST(WaartsTests, Resistance25QuadraticTermsFORM)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsFORM();
}

TEST(WaartsTests, Resistance25QuadraticTermsCrudeMonteCarlo)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, Resistance25QuadraticTermsDirectionalSampling)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, Resistance25QuadraticTermsImportanceSampling)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, Resistance25QuadraticTermsAdaptiveImportanceSampling)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, Resistance25QuadraticTermsFDIR)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsFDIR();
}

TEST(WaartsTests, Resistance25QuadraticTermsDSFI)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsDSFI();
}

TEST(WaartsTests, ConvexFailureDomainFORM)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsFORM();
}

TEST(WaartsTests, ConvexFailureDomainCrudeMonteCarlo)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, ConvexFailureDomainDirectionalSampling)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, ConvexFailureDomainNumericalIntegration)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, ConvexFailureDomainImportanceSampling)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, ConvexFailureDomainAdaptiveImportanceSampling)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, ConvexFailureDomainFDIR)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsFDIR();
}

TEST(WaartsTests, ConvexFailureDomainDSFI)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsDSFI();
}

TEST(WaartsTests, OblateSpheroidFORM)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsFORM();
}

TEST(WaartsTests, OblateSpheroidCrudeMonteCarlo)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, OblateSpheroidDirectionalSampling)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, OblateSpheroidImportanceSampling)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, OblateSpheroidAdaptiveImportanceSampling)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, OblateSpheroidFDIR)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsFDIR();
}

TEST(WaartsTests, OblateSpheroidDSFI)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsDSFI();
}

TEST(WaartsTests, SaddleSurfaceFORM)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsFORM();
}

TEST(WaartsTests, SaddleSurfaceCrudeMonteCarlo)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, SaddleSurfaceDirectionalSampling)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, SaddleSurfaceNumericalIntegration)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, SaddleSurfaceImportanceSampling)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, SaddleSurfaceAdaptiveImportanceSampling)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, SaddleSurfaceFDIR)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsFDIR();
}

TEST(WaartsTests, SaddleSurfaceDSFI)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsDSFI();
}

TEST(WaartsTests, DiscontinuousFORM)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsFORM();
}

TEST(WaartsTests, DiscontinuousCrudeMonteCarlo)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, DiscontinuousDirectionalSampling)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, DiscontinuousNumericalIntegration)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, DiscontinuousImportanceSampling)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, DiscontinuousAdaptiveImportanceSampling)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, DiscontinuousFDIR)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsFDIR();
}

TEST(WaartsTests, DiscontinuousDSFI)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsDSFI();
}

TEST(WaartsTests, TwoBranchesFORM)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsFORM();
}

TEST(WaartsTests, TwoBranchesCrudeMonteCarlo)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, TwoBranchesDirectionalSampling)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, TwoBranchesNumericalIntegration)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, TwoBranchesAdaptiveImportanceSampling)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, TwoBranchesFDIR)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsFDIR();
}

TEST(WaartsTests, TwoBranchesDSFI)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsDSFI();
}

TEST(WaartsTests, ConcaveFailureDomainFORM)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsFORM();
}

TEST(WaartsTests, ConcaveFailureDomainCrudeMonteCarlo)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, ConcaveFailureDomainDirectionalSampling)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, ConcaveFailureDomainNumericalIntegration)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, ConcaveFailureDomainImportanceSampling)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, ConcaveFailureDomainAdaptiveImportanceSampling)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, ConcaveFailureDomainFDIR)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsFDIR();
}

TEST(WaartsTests, ConcaveFailureDomainDSFI)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsDSFI();
}

TEST(WaartsTests, SeriesSystemFORM)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsFORM();
}

TEST(WaartsTests, SeriesSystemCrudeMonteCarlo)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, SeriesSystemDirectionalSampling)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, SeriesSystemNumericalIntegration)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, SeriesSystemImportanceSampling)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, SeriesSystemAdaptiveImportanceSampling)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, SeriesSystemDomainFDIR)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsFDIR();
}

TEST(WaartsTests, SeriesSystemDomainDSFI)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsDSFI();
}

