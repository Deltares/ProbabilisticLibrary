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
#include "Reliability/TestStartPointCalculator.h"
#include "Reliability/TestReliabilityMethods.h"
#include "Math/testCholeskiDecomposition.h"
#include "Math/testNumericSupport.h"
#include "Math/testKMean.h"
#include "Math/testMatrixInverse.h"
#include "Math/testMatrixMultiplication.h"
#include "Math/testRandom.h"
#include "Math/testVector1D.h"
#include "Math/testRootfinder.h"
#include "Math/TestSpecialFunctions.h"
#include "Combin/hohenbichler_tests.h"
#include "Combin/intEqualElements_tests.h"
#include "Combin/UpscaleTests.h"
#include "Combin/combinElements_tests.h"
#include "Combin/combiner_tests.h"
#include "Statistics/TestStandardNormal.h"
#include "Statistics/TestCopula.h"
#include "Distributions/testDistributions.h"
#include "Statistics/TestXfromU.h"
#include "Math/testMatrix.h"
#include "Model/TestRunModel.h"
#include "Uncertainty/TestUncertainty.h"
#include "Sensitivity/TestSensitivity.h"
#include "Optimization/testCobyla.h"
#include "Proxies/TestProxies.h"
#include "Utils/TestExceptions.h"
#include "Reliability/Waarts/TestWaartsLinearResistance.h"
#include "Reliability/Waarts/TestWaartsNoisyLimitState.h"
#include "Reliability/Waarts/TestWaartsResistanceOneQuadraticTerm.h"
#include "Reliability/Waarts/TestWaartsResistanceTenQuadraticTerms.h"
#include "Reliability/Waarts/TestWaartsResistance25QuadraticTerms.h"
#include "Reliability/Waarts/TestWaartsConvexFailureDomain.h"
#include "Reliability/Waarts/TestWaartsOblateSpheroid.h"
#include "Reliability/Waarts/TestWaartsSaddleSurface.h"
#include "Reliability/Waarts/TestWaartsDiscontinuousLimitState.h"
#include "Reliability/Waarts/TestWaartsTwoBranches.h"
#include "Reliability/Waarts/TestWaartsConcaveFailureDomain.h"
#include "Reliability/Waarts/TestWaartsSeriesSystem.h"
#include "Reliability/Waarts/TestWaartsParallelSystem.h"
#include "Reliability/Waarts/TestWaartsResistance25QuadraticTermsSparse.h"

using namespace Deltares::Probabilistic::Test;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "ReliabilityMethodTest.testClustersAdpImpSampling";
    return RUN_ALL_TESTS();
}

TEST(UnitTests, testCombiner)
{
    auto h = CombinerTest();
    h.allCombinerTests();
}

TEST(UnitTests, testCombinTwoElements)
{
    auto h = combinElementsTests();
    h.runAllCombineTwoElementsTests();
}

TEST(UnitTests, testCombinMultipleElements)
{
    auto h = combinElementsTests();
    h.runAllCombineMultipleElementsTests();
}

TEST(UnitTests, testLengthEffect)
{
    auto h = combinElementsTests();
    h.runAllLengthEffectTests();
}

TEST(UnitTests, AllUpscaleInTimeTests)
{
    auto h = UpscaleTests();
    h.runAllUpscaleInTimeTests();
}

TEST(UnitTests, AllEquivalentAlphaTests)
{
    auto h = UpscaleTests();
    h.runAllEquivalentAlphaTests();
}

TEST(UnitTests, AllUpscaleToLargestBlockTests)
{
    auto h = UpscaleTests();
    h.runAllUpscaleToLargestBlockTests();
}

TEST(UnitTests, testIntEqualElements)
{
    auto h = intEqualElements_tests();
    h.allIntegrateEqualElementsTests();
}

TEST(UnitTests, testHohenbichler)
{
    auto h = HohenbichlerTest();
    h.allHohenbichlerTests();
}

TEST(UnitTests, testCholeskiDecomp)
{
    auto tstCholeskiDecomp = choleski_decomp_tests();
    tstCholeskiDecomp.allCholeskyDecompositionTests();
}

TEST(UnitTests, testMatinv)
{
    auto tstMatInv = matinv_tests();
    tstMatInv.all_matinv_tests();
}

TEST(UnitTests, testMatmul)
{
    auto tstMatMul = matmul_tests();
    tstMatMul.all_matmul_tests();
}

TEST(MatrixTest, qr_decomposition)
{
    auto testMatrix = matrix_tests();
    testMatrix.qr_decomposition();
}

TEST(MatrixTest, linear_equations)
{
    auto testMatrix = matrix_tests();
    testMatrix.linear_equations();
}

TEST(MatrixTest, linear_equations_overdetermined)
{
    auto testMatrix = matrix_tests();
    testMatrix.linear_equations_overdetermined();
}

TEST(ProxiesTest, linear_model)
{
    auto testProxies = TestProxies();
    testProxies.testLinearModel();
}

TEST(ProxiesTest, linear_output_only_model)
{
    auto testProxies = TestProxies();
    testProxies.testLinearOutputOnlyModel();
}

TEST(UnitTests, testNumericalSupport)
{
    testNumericSupport::allNumericSupportTests();
}

TEST(UnitTests, testKMeanClustering)
{
    auto h = testKMean();
    h.allKMeanTests();
}

TEST(UnitTests, testStartPointCalculator)
{
    auto tstStartPoint = TestStartPointCalculator();
    tstStartPoint.allStartPointTests();
}

TEST(UnitTests, testExceptions)
{
    TestExceptions::testProblibExceptions();
}

TEST(ReliabilityMethodTest, TestFORM)
{
    TestReliabilityMethods::testFORM();
}

TEST(ReliabilityMethodTest, TestFORMArray)
{
    TestReliabilityMethods::testFORMArray();
}

TEST(ReliabilityMethodTest, TestFORMVaryingArray)
{
    TestReliabilityMethods::testFORMVaryingArray();
}

TEST(ReliabilityMethodTest, TestValidation)
{
    TestReliabilityMethods::testValidation();
}

TEST(ReliabilityMethodTest, TestLatinHyperCube)
{
    TestReliabilityMethods::testLatinHyperCube();
}

TEST(ReliabilityMethodTest, TestCobylaReliability)
{
    TestReliabilityMethods::testCobylaReliability();
}

TEST(ReliabilityMethodTest, TestNumBisection1)
{
    TestReliabilityMethods::testNumericalBisection();
}

TEST(ReliabilityMethodTest, TestNumBisection2)
{
    TestReliabilityMethods::testNumericalBisectionLinear();
}

TEST(ReliabilityMethodTest, TestSubSetSimulationReliabilityNearestToMean)
{
    TestReliabilityMethods::testSubSetSimulationReliabilityNearestToMean();
}

TEST(ReliabilityMethodTest, TestSubSetSimulationReliabilityCenterOfGravity)
{
    TestReliabilityMethods::testSubSetSimulationReliabilityCenterOfGravity();
}

TEST(ReliabilityMethodTest, TestFDIRReliability)
{
    TestReliabilityMethods::testFDIRReliability();
}

TEST(ReliabilityMethodTest, TestDSFIReliability)
{
    TestReliabilityMethods::testDSFIReliability();
}

TEST(ReliabilityMethodTest, TestFragilityCurveIntegration)
{
    TestReliabilityMethods::testFragilityCurveIntegration();
}

TEST(ReliabilityMethodTest, TestNumericalIntegrationReliability)
{
    TestReliabilityMethods::testNumericalIntegrationReliability();
}

TEST(ReliabilityMethodTest, TestCrudeMCReliability)
{
    TestReliabilityMethods::testCrudeMonteCarloReliability();
}

TEST(ReliabilityMethodTest, testCrudeMCwithCopula)
{
    TestReliabilityMethods::testCrudeMonteCarloWithCopulaReliability();
}

TEST(ReliabilityMethodTest, TestAdaptiveImportanceSampling)
{
    TestReliabilityMethods::testAdaptiveImportanceSampling();
}

TEST(ReliabilityMethodTest, TestClustersAdpImpSampling)
{
    TestReliabilityMethods::testClustersAdpImpSampling();
}

TEST(ReliabilityMethodTest, TestDirSamplingTwoBranches)
{
    TestReliabilityMethods::testDirSamplingProxyModels(false, ModelVaryingType::Monotone, 1.0);
}

TEST(ReliabilityMethodTest, TestDirSamplingProxyModels)
{
    TestReliabilityMethods::testDirSamplingProxyModels(true, ModelVaryingType::Monotone, 1.0);
}

TEST(ReliabilityMethodTest, TestDirSamplingTwoBranchesNotMonotone)
{
    TestReliabilityMethods::testDirSamplingProxyModels(false, ModelVaryingType::Varying, 1.0);
}

TEST(ReliabilityMethodTest, TestDirSamplingProxyModelsNotMonotone)
{
    TestReliabilityMethods::testDirSamplingProxyModels(true, ModelVaryingType::Varying, 1.0);
}

TEST(ReliabilityMethodTest, TestDirSamplingTwoBranchesLargerStepsize)
{
    TestReliabilityMethods::testDirSamplingProxyModels(false, ModelVaryingType::Monotone, 3.0);
}
TEST(DistributionTests, testConditionalStochast)
{
    testDistributions::testConditionalStochast();
}

TEST(DistributionTests, testComposite)
{
    testDistributions::testComposite();
}

TEST(DistributionTests, testCompositeConditionalStochast)
{
    testDistributions::testCompositeConditionalStochast();
}

TEST(DistributionTests, testCompositeConditionalTruncatedStochast)
{
    testDistributions::testCompositeConditionalTruncatedStochast();
}

TEST(DistributionTests, testCompositeStochast)
{
    testDistributions::testCompositeStochast();
}

TEST(DistributionTests, testCompositeTruncatedStochast)
{
    testDistributions::testCompositeTruncatedStochast();
}

TEST(DistributionTests, testConditionalWeibull)
{
    testDistributions::testConditionalWeibull();
}

TEST(DistributionTests, testConditionalWeibullCdfPdf)
{
    testDistributions::testConditionalWeibullCdfPdf();
}

TEST(DistributionTests, testConditionalWeibullCdfPdf2)
{
    testDistributions::testConditionalWeibullCdfPdf2();
}

TEST(DistributionTests, testConditionalWeibullMeanDeviation)
{
    testDistributions::testConditionalWeibullMeanDeviation();
}

TEST(DistributionTests, testConditionalWeibullNonIntegerShape)
{
    testDistributions::testConditionalWeibullNonIntegerShape();
}

TEST(DistributionTests, testDesignValue)
{
    testDistributions::testDesignValue();
}

TEST(DistributionTests, testNormal)
{
    testDistributions::testNormal();
}

TEST(DistributionTests, testLogNormal)
{
    testDistributions::testLogNormal();
}

TEST(DistributionTests, testUniform)
{
    testDistributions::testUniform();
}

TEST(DistributionTests, testTriangular)
{
    testDistributions::testTriangular();
}

TEST(DistributionTests, testTrapezoidal)
{
    testDistributions::testTrapezoidal();
}

TEST(DistributionTests, testExponential)
{
    testDistributions::testExponential();
}

TEST(DistributionTests, testGamma)
{
    testDistributions::testGamma();
}

TEST(DistributionTests, testBeta)
{
    testDistributions::testBeta();
}

TEST(DistributionTests, testGumbel)
{
    testDistributions::testGumbel();
}

TEST(DistributionTests, testWeibull)
{
    testDistributions::testWeibull();
}

TEST(DistributionTests, testFrechet)
{
    testDistributions::testFrechet();
}

TEST(DistributionTests, testGEV)
{
    testDistributions::testGEV();
}

TEST(DistributionTests, testPareto)
{
    testDistributions::testPareto();
}

TEST(DistributionTests, testGeneralizedPareto)
{
    testDistributions::testGeneralizedPareto();
}

TEST(DistributionTests, testRayleigh)
{
    testDistributions::testRayleigh();
}

TEST(DistributionTests, testRayleighN)
{
    testDistributions::testRayleighN();
}

TEST(DistributionTests, testBernoulli)
{
    testDistributions::testBernoulli();
}

TEST(DistributionTests, testPoisson)
{
    testDistributions::testPoisson();
}

TEST(DistributionTests, testStudentT)
{
    testDistributions::testStudentT();
}

TEST(DistributionTests, testStudentTwithInterpolation)
{
    testDistributions::testStudentTwithInterpolation();
}

TEST(DistributionTests, testValidation)
{
    testDistributions::testValidation();
}

TEST(DistributionTests, testVariationCoefficient)
{
    testDistributions::testVariationCoefficient();
}

TEST(XfromUTests, testNormal)
{
    TestXfromU::TestNormal();
}

TEST(XfromUTests, testUniform)
{
    TestXfromU::TestUniform();
}

TEST(XfromUTests, testTriangular)
{
    TestXfromU::TestTriangular();
}

TEST(XfromUTests, testExponential)
{
    TestXfromU::TestExponential();
}

TEST(XfromUTests, testGumbel)
{
    TestXfromU::TestGumbel();
}

TEST(XfromUTests, testWeibull)
{
    TestXfromU::TestWeibull();
}

TEST(XfromUTests, testPareto)
{
    TestXfromU::TestPareto();
}

TEST(XfromUTests, testRayleigh)
{
    TestXfromU::TestRayleigh();
}

TEST(XfromUTests, testRayleighN)
{
    TestXfromU::TestRayleighN();
}

TEST(XfromUTests, testTruncatedNormal)
{
    TestXfromU::TestTruncatedNormal();
}

TEST(UnitTests, testStandardNormal)
{
    auto tstStdNormal = TestStandardNormal();
    tstStdNormal.allStandardNormalTests();
}

TEST(CopulasTest, testClayton)
{
    TestCopula::testClayton();
}

TEST(CopulasTest, testFrank)
{
    TestCopula::testFrank();
}

TEST(CopulasTest, testGaussian)
{
    TestCopula::testGaussian();
}

TEST(CopulasTest, testGumbel)
{
    TestCopula::testGumbel();
}

TEST(CopulasTest, testValidation)
{
    TestCopula::testValidation();
}

TEST(CopulasTest, testValidationMessages)
{
    TestCopula::testValidationMessages();
}

TEST(CopulasTest, testGaussianValidationMessages)
{
    TestCopula::testGaussianValidationMessages();
}

TEST(UnitTests, testUncertainty)
{
    auto tstUncertainty = TestUncertainty();
    tstUncertainty.allUncertaintyTests();
}

TEST(UnitTests, testSensitivity)
{
    auto tstSensitivity = TestSensitivity();
    tstSensitivity.allSensitivityTests();
}

TEST(UnitTests, testRunModel)
{
    auto tstModel = TestRunModel();
    tstModel.allModelTests();
}

TEST(UnitTests, testRandom)
{
    testRandom::allRandomTests();
}

TEST(UnitTests, testRootfinder)
{
    auto tstRF = rootfinder_tests();
    tstRF.all_rootfinder_tests();
}

TEST(UnitTests, testVector1D)
{
    auto tstVector1D = vector1D_tests();
    tstVector1D.allVector1Dtests();
}

TEST(UnitTests, testIncompleteGamma)
{
    TestSpecialFunctions::incompleteGammaTest();
}

TEST(UnitTests, testIncompleteGammaExceptionalCases)
{
    TestSpecialFunctions::incompleteGammaExceptionalCasesTest();
}

TEST(UnitTests, testCobylaOptimization)
{
    auto tstCobylaOpt = testCobyla();
    tstCobylaOpt.allCobylaTests();
}

TEST(WaartsTests, testLinearResistanceFORM)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsFORM();
}

TEST(WaartsTests, testLinearResistanceCrudeMonteCarlo)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testLinearResistanceDirectionalSampling)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testLinearResistanceNumericalIntegration)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testLinearResistanceNumericalIntegrationCenterOfGravity)
{
    auto tester = TestWaartsLinearResistance();
    tester.runNumInt(CenterOfGravity);
}

TEST(WaartsTests, testLinearResistanceNumericalIntegrationCenterOfAngles)
{
    auto tester = TestWaartsLinearResistance();
    tester.runNumInt(CenterOfAngles);
}

TEST(WaartsTests, testLinearResistanceNumericalIntegrationNearestToMean)
{
    auto tester = TestWaartsLinearResistance();
    tester.runNumInt(NearestToMean);
}

TEST(WaartsTests, testLinearResistanceImportanceSampling)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testLinearResistanceAdaptiveImportanceSampling)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testLinearResistanceFDIR)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testLinearResistanceDSFI)
{
    auto tester = TestWaartsLinearResistance();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testNoisyLimitStateFORM)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsFORM();
}

TEST(WaartsTests, testNoisyLimitStateCrudeMonteCarlo)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testNoisyLimitStateDirectionalSampling)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testNoisyLimitStateImportanceSampling)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testNoisyLimitStateAdaptiveImportanceSampling)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testNoisyLimitStateFDIR)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testNoisyLimitStateDSFI)
{
    auto tester = TestWaartsNoisyLimitState();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testResistanceOneQuadraticFORM)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsFORM();
}

TEST(WaartsTests, testResistanceOneQuadraticCrudeMonteCarlo)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testResistanceOneQuadraticDirectionalSampling)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testResistanceOneQuadraticNumericalIntegration)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testResistanceOneQuadraticImportanceSampling)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testResistanceOneQuadraticAdaptiveImportanceSampling)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testResistanceOneQuadraticFDIR)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testResistanceOneQuadraticDSFI)
{
    auto tester = TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsFORM)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsFORM();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsCrudeMonteCarlo)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsDirectionalSampling)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsImportanceSampling)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsAdaptiveImportanceSampling)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsFDIR)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsDSFI)
{
    auto tester = TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testResistance25QuadraticTermsFORM)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsFORM();
}

TEST(WaartsTests, testResistance25QuadraticTermsCrudeMonteCarlo)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testResistance25QuadraticTermsDirectionalSampling)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsImportanceSampling)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsAdaptiveImportanceSampling)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsFDIR)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testResistance25QuadraticTermsDSFI)
{
    auto tester = TestWaartsResistance25QuadraticTerms();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testConvexFailureDomainFORM)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsFORM();
}

TEST(WaartsTests, testConvexFailureDomainCrudeMonteCarlo)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testConvexFailureDomainDirectionalSampling)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testConvexFailureDomainNumericalIntegration)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testConvexFailureDomainImportanceSampling)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testConvexFailureDomainAdaptiveImportanceSampling)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testConvexFailureDomainFDIR)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testConvexFailureDomainDSFI)
{
    auto tester = TestWaartsConvexFailureDomain();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testOblateSpheroidFORM)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsFORM();
}

TEST(WaartsTests, testOblateSpheroidCrudeMonteCarlo)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testOblateSpheroidDirectionalSampling)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testOblateSpheroidImportanceSampling)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testOblateSpheroidAdaptiveImportanceSampling)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testOblateSpheroidFDIR)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testOblateSpheroidDSFI)
{
    auto tester = TestWaartsOblateSpheroid();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testSaddleSurfaceFORM)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsFORM();
}

TEST(WaartsTests, testSaddleSurfaceCrudeMonteCarlo)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testSaddleSurfaceDirectionalSampling)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testSaddleSurfaceNumericalIntegration)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testSaddleSurfaceImportanceSampling)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testSaddleSurfaceAdaptiveImportanceSampling)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testSaddleSurfaceFDIR)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testSaddleSurfaceDSFI)
{
    auto tester = TestWaartsSaddleSurface();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testDiscontinuousFORM)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsFORM();
}

TEST(WaartsTests, testDiscontinuousCrudeMonteCarlo)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testDiscontinuousDirectionalSampling)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testDiscontinuousNumericalIntegration)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testDiscontinuousImportanceSampling)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testDiscontinuousAdaptiveImportanceSampling)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testDiscontinuousFDIR)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testDiscontinuousDSFI)
{
    auto tester = TestWaartsDiscontinuousLimitState();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testTwoBranchesFORM)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsFORM();
}

TEST(WaartsTests, testTwoBranchesCrudeMonteCarlo)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testTwoBranchesDirectionalSampling)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testTwoBranchesNumericalIntegration)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testTwoBranchesAdaptiveImportanceSampling)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testTwoBranchesFDIR)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testTwoBranchesDSFI)
{
    auto tester = TestWaartsTwoBranches();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testConcaveFailureDomainFORM)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsFORM();
}

TEST(WaartsTests, testConcaveFailureDomainCrudeMonteCarlo)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testConcaveFailureDomainDirectionalSampling)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testConcaveFailureDomainNumericalIntegration)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testConcaveFailureDomainImportanceSampling)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testConcaveFailureDomainAdaptiveImportanceSampling)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testConcaveFailureDomainFDIR)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testConcaveFailureDomainDSFI)
{
    auto tester = TestWaartsConcaveFailureDomain();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testSeriesSystemFORM)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsFORM();
}

TEST(WaartsTests, testSeriesSystemCrudeMonteCarlo)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testSeriesSystemDirectionalSampling)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testSeriesSystemNumericalIntegration)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testSeriesSystemImportanceSampling)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testSeriesSystemAdaptiveImportanceSampling)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testSeriesSystemDomainFDIR)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testSeriesSystemDomainDSFI)
{
    auto tester = TestWaartsSeriesSystem();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testParallelSystemFORM)
{
    auto tester = TestWaartsParallelSystem();
    tester.WaartsFORM();
}

TEST(WaartsTests, testParallelSystemCrudeMonteCarlo)
{
    auto tester = TestWaartsParallelSystem();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testParallelSystemDirectionalSampling)
{
    auto tester = TestWaartsParallelSystem();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testParallelSystemImportanceSampling)
{
    auto tester = TestWaartsParallelSystem();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testParallelSystemAdaptiveImportanceSampling)
{
    auto tester = TestWaartsParallelSystem();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testParallelSystemDomainFDIR)
{
    auto tester = TestWaartsParallelSystem();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testParallelSystemDomainDSFI)
{
    auto tester = TestWaartsParallelSystem();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseFORM)
{
    auto tester = TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsFORM();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseCrudeMonteCarlo)
{
    auto tester = TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseDirectionalSampling)
{
    auto tester = TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseImportanceSampling)
{
    auto tester = TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseAdaptiveImportanceSampling)
{
    auto tester = TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseFDIR)
{
    auto tester = TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseDSFI)
{
    auto tester = TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsDSFI();
}

