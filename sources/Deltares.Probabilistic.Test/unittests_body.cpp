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
#include "Statistics/TestXfromU.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = Deltares::Probabilistic::Test::"ReliabilityMethodTest.testClustersAdpImpSampling";
    return RUN_ALL_TESTS();
}

TEST(UnitTests, testCombiner)
{
    auto h = Deltares::Probabilistic::Test::CombinerTest();
    h.allCombinerTests();
}

TEST(UnitTests, testCombinTwoElements)
{
    auto h = Deltares::Probabilistic::Test::combinElementsTests();
    h.runAllCombineTwoElementsTests();
}

TEST(UnitTests, testCombinMultipleElements)
{
    auto h = Deltares::Probabilistic::Test::combinElementsTests();
    h.runAllCombineMultipleElementsTests();
}

TEST(UnitTests, testLengthEffect)
{
    auto h = Deltares::Probabilistic::Test::combinElementsTests();
    h.runAllLengthEffectTests();
}

TEST(UnitTests, AllUpscaleInTimeTests)
{
    auto h = Deltares::Probabilistic::Test::UpscaleTests();
    h.runAllUpscaleInTimeTests();
}

TEST(UnitTests, AllEquivalentAlphaTests)
{
    auto h = Deltares::Probabilistic::Test::UpscaleTests();
    h.runAllEquivalentAlphaTests();
}

TEST(UnitTests, AllUpscaleToLargestBlockTests)
{
    auto h = Deltares::Probabilistic::Test::UpscaleTests();
    h.runAllUpscaleToLargestBlockTests();
}

TEST(UnitTests, testIntEqualElements)
{
    auto h = Deltares::Probabilistic::Test::intEqualElements_tests();
    h.allIntegrateEqualElementsTests();
}

TEST(UnitTests, testHohenbichler)
{
    auto h = Deltares::Probabilistic::Test::HohenbichlerTest();
    h.allHohenbichlerTests();
}

TEST(UnitTests, testCholeskiDecomp)
{
    auto tstCholeskiDecomp = Deltares::Probabilistic::Test::choleski_decomp_tests();
    tstCholeskiDecomp.allCholeskyDecompositionTests();
}

TEST(UnitTests, testMatinv)
{
    auto tstMatInv = Deltares::Probabilistic::Test::matinv_tests();
    tstMatInv.all_matinv_tests();
}

TEST(UnitTests, testMatmul)
{
    auto tstMatMul = Deltares::Probabilistic::Test::matmul_tests();
    tstMatMul.all_matmul_tests();
}

TEST(MatrixTest, qr_decomposition)
{
    auto testMatrix = Deltares::Probabilistic::Test::matrix_tests();
    testMatrix.qr_decomposition();
}

TEST(MatrixTest, linear_equations)
{
    auto testMatrix = Deltares::Probabilistic::Test::matrix_tests();
    testMatrix.linear_equations();
}

TEST(MatrixTest, linear_equations_overdetermined)
{
    auto testMatrix = Deltares::Probabilistic::Test::matrix_tests();
    testMatrix.linear_equations_overdetermined();
}

TEST(ProxiesTest, linear_model)
{
    auto testProxies = Deltares::Probabilistic::Test::TestProxies();
    testProxies.testLinearModel();
}

TEST(ProxiesTest, linear_output_only_model)
{
    auto testProxies = Deltares::Probabilistic::Test::TestProxies();
    testProxies.testLinearOutputOnlyModel();
}

TEST(UnitTests, testNumericalSupport)
{
    Deltares::Probabilistic::Test::testNumericSupport::allNumericSupportTests();
}

TEST(UnitTests, testKMeanClustering)
{
    auto h = Deltares::Probabilistic::Test::testKMean();
    h.allKMeanTests();
}

TEST(UnitTests, testStartPointCalculator)
{
    auto tstStartPoint = Deltares::Probabilistic::Test::TestStartPointCalculator();
    tstStartPoint.allStartPointTests();
}

TEST(UnitTests, testExceptions)
{
    Deltares::Probabilistic::Test::TestExceptions::testProblibExceptions();
}

TEST(ReliabilityMethodTest, TestFORM)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testFORM();
}

TEST(ReliabilityMethodTest, TestFORMArray)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testFORMArray();
}

TEST(ReliabilityMethodTest, TestFORMVaryingArray)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testFORMVaryingArray();
}

TEST(ReliabilityMethodTest, TestValidation)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testValidation();
}

TEST(ReliabilityMethodTest, TestLatinHyperCube)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testLatinHyperCube();
}

TEST(ReliabilityMethodTest, TestCobylaReliability)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testCobylaReliability();
}

TEST(ReliabilityMethodTest, TestNumBisection1)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testNumericalBisection();
}

TEST(ReliabilityMethodTest, TestNumBisection2)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testNumericalBisectionLinear();
}

TEST(ReliabilityMethodTest, TestSubSetSimulationReliabilityNearestToMean)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testSubSetSimulationReliabilityNearestToMean();
}

TEST(ReliabilityMethodTest, TestSubSetSimulationReliabilityCenterOfGravity)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testSubSetSimulationReliabilityCenterOfGravity();
}

TEST(ReliabilityMethodTest, TestFDIRReliability)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testFDIRReliability();
}

TEST(ReliabilityMethodTest, TestDSFIReliability)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testDSFIReliability();
}

TEST(ReliabilityMethodTest, TestFragilityCurveIntegration)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testFragilityCurveIntegration();
}

TEST(ReliabilityMethodTest, TestNumericalIntegrationReliability)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testNumericalIntegrationReliability();
}

TEST(ReliabilityMethodTest, TestCrudeMCReliability)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testCrudeMonteCarloReliability();
}

TEST(ReliabilityMethodTest, testCrudeMCwithCopula)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testCrudeMonteCarloWithCopulaReliability();
}

TEST(ReliabilityMethodTest, TestAdaptiveImportanceSampling)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testAdaptiveImportanceSampling();
}

TEST(ReliabilityMethodTest, TestClustersAdpImpSampling)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testClustersAdpImpSampling();
}

TEST(ReliabilityMethodTest, TestDirSamplingTwoBranches)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testDirSamplingProxyModels(false, ModelVaryingType::Monotone, 1.0);
}

TEST(ReliabilityMethodTest, TestDirSamplingProxyModels)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testDirSamplingProxyModels(true, ModelVaryingType::Monotone, 1.0);
}

TEST(ReliabilityMethodTest, TestDirSamplingTwoBranchesNotMonotone)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testDirSamplingProxyModels(false, ModelVaryingType::Varying, 1.0);
}

TEST(ReliabilityMethodTest, TestDirSamplingProxyModelsNotMonotone)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testDirSamplingProxyModels(true, ModelVaryingType::Varying, 1.0);
}

TEST(ReliabilityMethodTest, TestDirSamplingTwoBranchesLargerStepsize)
{
    Deltares::Probabilistic::Test::TestReliabilityMethods::testDirSamplingProxyModels(false, ModelVaryingType::Monotone, 3.0);
}
TEST(DistributionTests, testConditionalStochast)
{
    Deltares::Probabilistic::Test::testDistributions::testConditionalStochast();
}

TEST(DistributionTests, testComposite)
{
    Deltares::Probabilistic::Test::testDistributions::testComposite();
}

TEST(DistributionTests, testCompositeConditionalStochast)
{
    Deltares::Probabilistic::Test::testDistributions::testCompositeConditionalStochast();
}

TEST(DistributionTests, testCompositeConditionalTruncatedStochast)
{
    Deltares::Probabilistic::Test::testDistributions::testCompositeConditionalTruncatedStochast();
}

TEST(DistributionTests, testCompositeStochast)
{
    Deltares::Probabilistic::Test::testDistributions::testCompositeStochast();
}

TEST(DistributionTests, testCompositeTruncatedStochast)
{
    Deltares::Probabilistic::Test::testDistributions::testCompositeTruncatedStochast();
}

TEST(DistributionTests, testConditionalWeibull)
{
    Deltares::Probabilistic::Test::testDistributions::testConditionalWeibull();
}

TEST(DistributionTests, testConditionalWeibullCdfPdf)
{
    Deltares::Probabilistic::Test::testDistributions::testConditionalWeibullCdfPdf();
}

TEST(DistributionTests, testConditionalWeibullCdfPdf2)
{
    Deltares::Probabilistic::Test::testDistributions::testConditionalWeibullCdfPdf2();
}

TEST(DistributionTests, testConditionalWeibullMeanDeviation)
{
    Deltares::Probabilistic::Test::testDistributions::testConditionalWeibullMeanDeviation();
}

TEST(DistributionTests, testConditionalWeibullNonIntegerShape)
{
    Deltares::Probabilistic::Test::testDistributions::testConditionalWeibullNonIntegerShape();
}

TEST(DistributionTests, testDesignValue)
{
    Deltares::Probabilistic::Test::testDistributions::testDesignValue();
}

TEST(DistributionTests, testNormal)
{
    Deltares::Probabilistic::Test::testDistributions::testNormal();
}

TEST(DistributionTests, testLogNormal)
{
    Deltares::Probabilistic::Test::testDistributions::testLogNormal();
}

TEST(DistributionTests, testUniform)
{
    Deltares::Probabilistic::Test::testDistributions::testUniform();
}

TEST(DistributionTests, testTriangular)
{
    Deltares::Probabilistic::Test::testDistributions::testTriangular();
}

TEST(DistributionTests, testTrapezoidal)
{
    Deltares::Probabilistic::Test::testDistributions::testTrapezoidal();
}

TEST(DistributionTests, testExponential)
{
    Deltares::Probabilistic::Test::testDistributions::testExponential();
}

TEST(DistributionTests, testGamma)
{
    Deltares::Probabilistic::Test::testDistributions::testGamma();
}

TEST(DistributionTests, testBeta)
{
    Deltares::Probabilistic::Test::testDistributions::testBeta();
}

TEST(DistributionTests, testGumbel)
{
    Deltares::Probabilistic::Test::testDistributions::testGumbel();
}

TEST(DistributionTests, testWeibull)
{
    Deltares::Probabilistic::Test::testDistributions::testWeibull();
}

TEST(DistributionTests, testFrechet)
{
    Deltares::Probabilistic::Test::testDistributions::testFrechet();
}

TEST(DistributionTests, testGEV)
{
    Deltares::Probabilistic::Test::testDistributions::testGEV();
}

TEST(DistributionTests, testPareto)
{
    Deltares::Probabilistic::Test::testDistributions::testPareto();
}

TEST(DistributionTests, testGeneralizedPareto)
{
    Deltares::Probabilistic::Test::testDistributions::testGeneralizedPareto();
}

TEST(DistributionTests, testRayleigh)
{
    Deltares::Probabilistic::Test::testDistributions::testRayleigh();
}

TEST(DistributionTests, testRayleighN)
{
    Deltares::Probabilistic::Test::testDistributions::testRayleighN();
}

TEST(DistributionTests, testBernoulli)
{
    Deltares::Probabilistic::Test::testDistributions::testBernoulli();
}

TEST(DistributionTests, testPoisson)
{
    Deltares::Probabilistic::Test::testDistributions::testPoisson();
}

TEST(DistributionTests, testStudentT)
{
    Deltares::Probabilistic::Test::testDistributions::testStudentT();
}

TEST(DistributionTests, testStudentTwithInterpolation)
{
    Deltares::Probabilistic::Test::testDistributions::testStudentTwithInterpolation();
}

TEST(DistributionTests, testValidation)
{
    Deltares::Probabilistic::Test::testDistributions::testValidation();
}

TEST(DistributionTests, testVariationCoefficient)
{
    Deltares::Probabilistic::Test::testDistributions::testVariationCoefficient();
}

TEST(XfromUTests, testNormal)
{
    Deltares::Probabilistic::Test::TestXfromU::TestNormal();
}

TEST(XfromUTests, testUniform)
{
    Deltares::Probabilistic::Test::TestXfromU::TestUniform();
}

TEST(XfromUTests, testTriangular)
{
    Deltares::Probabilistic::Test::TestXfromU::TestTriangular();
}

TEST(XfromUTests, testExponential)
{
    Deltares::Probabilistic::Test::TestXfromU::TestExponential();
}

TEST(XfromUTests, testGumbel)
{
    Deltares::Probabilistic::Test::TestXfromU::TestGumbel();
}

TEST(XfromUTests, testWeibull)
{
    Deltares::Probabilistic::Test::TestXfromU::TestWeibull();
}

TEST(XfromUTests, testPareto)
{
    Deltares::Probabilistic::Test::TestXfromU::TestPareto();
}

TEST(XfromUTests, testRayleigh)
{
    Deltares::Probabilistic::Test::TestXfromU::TestRayleigh();
}

TEST(XfromUTests, testRayleighN)
{
    Deltares::Probabilistic::Test::TestXfromU::TestRayleighN();
}

TEST(XfromUTests, testTruncatedNormal)
{
    Deltares::Probabilistic::Test::TestXfromU::TestTruncatedNormal();
}

TEST(UnitTests, testStandardNormal)
{
    auto tstStdNormal = Deltares::Probabilistic::Test::TestStandardNormal();
    tstStdNormal.allStandardNormalTests();
}

TEST(CopulasTest, testClayton)
{
    Deltares::Probabilistic::Test::TestCopula::testClayton();
}

TEST(CopulasTest, testFrank)
{
    Deltares::Probabilistic::Test::TestCopula::testFrank();
}

TEST(CopulasTest, testFrankExtremeTheta)
{
    Deltares::Probabilistic::Test::TestCopula::testFrankExtremeTheta();
}

TEST(CopulasTest, testGaussian)
{
    Deltares::Probabilistic::Test::TestCopula::testGaussian();
}

TEST(CopulasTest, testGumbel)
{
    Deltares::Probabilistic::Test::TestCopula::testGumbel();
}

TEST(CopulasTest, testValidation)
{
    Deltares::Probabilistic::Test::TestCopula::testValidation();
}

TEST(CopulasTest, testValidationMessages)
{
    Deltares::Probabilistic::Test::TestCopula::testValidationMessages();
}

TEST(CopulasTest, testGaussianValidationMessages)
{
    Deltares::Probabilistic::Test::TestCopula::testGaussianValidationMessages();
}

TEST(UnitTests, testUncertainty)
{
    auto tstUncertainty = Deltares::Probabilistic::Test::TestUncertainty();
    tstUncertainty.allUncertaintyTests();
}

TEST(UnitTests, testSensitivity)
{
    auto tstSensitivity = Deltares::Probabilistic::Test::TestSensitivity();
    tstSensitivity.allSensitivityTests();
}

TEST(UnitTests, testRunModel)
{
    auto tstModel = Deltares::Probabilistic::Test::TestRunModel();
    tstModel.allModelTests();
}

TEST(UnitTests, testRandom)
{
    Deltares::Probabilistic::Test::testRandom::allRandomTests();
}

TEST(UnitTests, testRootfinder)
{
    auto tstRF = Deltares::Probabilistic::Test::rootfinder_tests();
    tstRF.all_rootfinder_tests();
}

TEST(UnitTests, testVector1D)
{
    auto tstVector1D = Deltares::Probabilistic::Test::vector1D_tests();
    tstVector1D.allVector1Dtests();
}

TEST(UnitTests, testIncompleteGamma)
{
    Deltares::Probabilistic::Test::TestSpecialFunctions::incompleteGammaTest();
}

TEST(UnitTests, testIncompleteGammaExceptionalCases)
{
    Deltares::Probabilistic::Test::TestSpecialFunctions::incompleteGammaExceptionalCasesTest();
}

TEST(UnitTests, testCobylaOptimization)
{
    auto tstCobylaOpt = Deltares::Probabilistic::Test::testCobyla();
    tstCobylaOpt.allCobylaTests();
}

TEST(WaartsTests, testLinearResistanceFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.WaartsFORM();
}

TEST(WaartsTests, testLinearResistanceCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testLinearResistanceDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testLinearResistanceNumericalIntegration)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testLinearResistanceNumericalIntegrationCenterOfGravity)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.runNumInt(CenterOfGravity);
}

TEST(WaartsTests, testLinearResistanceNumericalIntegrationCenterOfAngles)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.runNumInt(CenterOfAngles);
}

TEST(WaartsTests, testLinearResistanceNumericalIntegrationNearestToMean)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.runNumInt(NearestToMean);
}

TEST(WaartsTests, testLinearResistanceImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testLinearResistanceAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testLinearResistanceFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testLinearResistanceDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsLinearResistance();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testNoisyLimitStateFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsNoisyLimitState();
    tester.WaartsFORM();
}

TEST(WaartsTests, testNoisyLimitStateCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsNoisyLimitState();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testNoisyLimitStateDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsNoisyLimitState();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testNoisyLimitStateImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsNoisyLimitState();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testNoisyLimitStateAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsNoisyLimitState();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testNoisyLimitStateFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsNoisyLimitState();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testNoisyLimitStateDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsNoisyLimitState();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testResistanceOneQuadraticFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsFORM();
}

TEST(WaartsTests, testResistanceOneQuadraticCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testResistanceOneQuadraticDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testResistanceOneQuadraticNumericalIntegration)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testResistanceOneQuadraticImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testResistanceOneQuadraticAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testResistanceOneQuadraticFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testResistanceOneQuadraticDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceOneQuadraticTerm();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsFORM();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testResistanceTenQuadraticTermsDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistanceTenQuadraticTerms();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testResistance25QuadraticTermsFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTerms();
    tester.WaartsFORM();
}

TEST(WaartsTests, testResistance25QuadraticTermsCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTerms();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testResistance25QuadraticTermsDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTerms();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTerms();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTerms();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTerms();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testResistance25QuadraticTermsDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTerms();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testConvexFailureDomainFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConvexFailureDomain();
    tester.WaartsFORM();
}

TEST(WaartsTests, testConvexFailureDomainCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConvexFailureDomain();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testConvexFailureDomainDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConvexFailureDomain();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testConvexFailureDomainNumericalIntegration)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConvexFailureDomain();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testConvexFailureDomainImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConvexFailureDomain();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testConvexFailureDomainAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConvexFailureDomain();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testConvexFailureDomainFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConvexFailureDomain();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testConvexFailureDomainDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConvexFailureDomain();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testOblateSpheroidFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsOblateSpheroid();
    tester.WaartsFORM();
}

TEST(WaartsTests, testOblateSpheroidCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsOblateSpheroid();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testOblateSpheroidDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsOblateSpheroid();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testOblateSpheroidImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsOblateSpheroid();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testOblateSpheroidAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsOblateSpheroid();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testOblateSpheroidFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsOblateSpheroid();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testOblateSpheroidDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsOblateSpheroid();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testSaddleSurfaceFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSaddleSurface();
    tester.WaartsFORM();
}

TEST(WaartsTests, testSaddleSurfaceCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSaddleSurface();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testSaddleSurfaceDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSaddleSurface();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testSaddleSurfaceNumericalIntegration)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSaddleSurface();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testSaddleSurfaceImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSaddleSurface();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testSaddleSurfaceAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSaddleSurface();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testSaddleSurfaceFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSaddleSurface();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testSaddleSurfaceDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSaddleSurface();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testDiscontinuousFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsDiscontinuousLimitState();
    tester.WaartsFORM();
}

TEST(WaartsTests, testDiscontinuousCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsDiscontinuousLimitState();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testDiscontinuousDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsDiscontinuousLimitState();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testDiscontinuousNumericalIntegration)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsDiscontinuousLimitState();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testDiscontinuousImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsDiscontinuousLimitState();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testDiscontinuousAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsDiscontinuousLimitState();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testDiscontinuousFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsDiscontinuousLimitState();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testDiscontinuousDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsDiscontinuousLimitState();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testTwoBranchesFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsTwoBranches();
    tester.WaartsFORM();
}

TEST(WaartsTests, testTwoBranchesCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsTwoBranches();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testTwoBranchesDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsTwoBranches();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testTwoBranchesNumericalIntegration)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsTwoBranches();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testTwoBranchesAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsTwoBranches();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testTwoBranchesFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsTwoBranches();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testTwoBranchesDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsTwoBranches();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testConcaveFailureDomainFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConcaveFailureDomain();
    tester.WaartsFORM();
}

TEST(WaartsTests, testConcaveFailureDomainCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConcaveFailureDomain();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testConcaveFailureDomainDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConcaveFailureDomain();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testConcaveFailureDomainNumericalIntegration)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConcaveFailureDomain();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testConcaveFailureDomainImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConcaveFailureDomain();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testConcaveFailureDomainAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConcaveFailureDomain();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testConcaveFailureDomainFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConcaveFailureDomain();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testConcaveFailureDomainDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsConcaveFailureDomain();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testSeriesSystemFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSeriesSystem();
    tester.WaartsFORM();
}

TEST(WaartsTests, testSeriesSystemCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSeriesSystem();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testSeriesSystemDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSeriesSystem();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testSeriesSystemNumericalIntegration)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSeriesSystem();
    tester.WaartsNumericalIntegration();
}

TEST(WaartsTests, testSeriesSystemImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSeriesSystem();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testSeriesSystemAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSeriesSystem();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testSeriesSystemDomainFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSeriesSystem();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testSeriesSystemDomainDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsSeriesSystem();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testParallelSystemFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsParallelSystem();
    tester.WaartsFORM();
}

TEST(WaartsTests, testParallelSystemCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsParallelSystem();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testParallelSystemDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsParallelSystem();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testParallelSystemImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsParallelSystem();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testParallelSystemAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsParallelSystem();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testParallelSystemDomainFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsParallelSystem();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testParallelSystemDomainDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsParallelSystem();
    tester.WaartsDSFI();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseFORM)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsFORM();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseCrudeMonteCarlo)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsCrudeMonteCarlo();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseDirectionalSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsDirectionalSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsImportanceSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseAdaptiveImportanceSampling)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsAdaptiveImportanceSampling();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseFDIR)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsFDIR();
}

TEST(WaartsTests, testResistance25QuadraticTermsSparseDSFI)
{
    auto tester = Deltares::Probabilistic::Test::TestWaartsResistance25QuadraticTermsSparse();
    tester.WaartsDSFI();
}

