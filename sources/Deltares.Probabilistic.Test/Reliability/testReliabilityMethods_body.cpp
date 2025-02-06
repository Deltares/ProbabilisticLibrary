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
#include <iostream>
#include <gtest/gtest.h>
#include "testReliabilityMethods.h"
#include "../../Deltares.Probabilistic/Reliability/LatinHyperCube.h"
#include "../../Deltares.Probabilistic/Reliability/NumericalBisection.h"
#include "../../Deltares.Probabilistic/Reliability/CobylaReliability.h"
#include "../../Deltares.Probabilistic/Reliability/SubsetSimulation.h"
#include "../../Deltares.Probabilistic/Reliability/FORMThenDirectionalSampling.h"
#include "../../Deltares.Probabilistic/Reliability/DirectionalSamplingThenFORM.h"
#include "../../Deltares.Probabilistic/Reliability/FragilityCurveIntegration.h"
#include "../../Deltares.Probabilistic/Reliability/NumericalIntegration.h"
#include "../projectBuilder.h"

using namespace Deltares::Reliability;
using namespace Deltares::Statistics;
using namespace Deltares::Models;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void testReliabilityMethods::testFORM()
            {
                auto calculator = FORM();

                auto modelRunner = projectBuilder().BuildLinearProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                EXPECT_NEAR(designPoint->Beta, 2.33, 1e-2);
                EXPECT_NEAR(designPoint->Alphas[0]->Alpha, -0.71, 1e-2);
                EXPECT_NEAR(designPoint->Alphas[1]->Alpha, -0.71, 1e-2);
                EXPECT_NEAR(designPoint->Alphas[0]->X, 0.9, 1e-2);
                EXPECT_NEAR(designPoint->Alphas[1]->X, 0.9, 1e-2);
            }

            void testReliabilityMethods::testFORMArray()
            {
                auto calculator = FORM();

                auto modelRunner = projectBuilder().BuildLinearArrayProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 10);
                EXPECT_NEAR(designPoint->Beta, 0.72, 1e-2);
                EXPECT_NEAR(designPoint->Alphas[0]->Alpha, -0.31, 1e-2);
                EXPECT_NEAR(designPoint->Alphas[5]->Alpha, -0.31, 1e-2);
                EXPECT_NEAR(designPoint->Alphas[0]->X, 0.18, 1e-2);
                EXPECT_NEAR(designPoint->Alphas[5]->X, 0.18, 1e-2);
            }

            void testReliabilityMethods::testLatinHyperCube()
            {
                const auto chunckSizes = std::vector<int>({ 1, 15, 2000 });
                for (const auto& chunkSize : chunckSizes)
                {
                    auto calculator = LatinHyperCube();
                    calculator.Settings->randomSettings->RandomGeneratorType = Numeric::RandomValueGeneratorType::MersenneTwister;
                    calculator.Settings->RunSettings->MaxChunkSize = chunkSize;

                    auto modelRunner = projectBuilder().BuildProject();

                    auto designPoint = calculator.getDesignPoint(modelRunner);

                    ASSERT_EQ(designPoint->Alphas.size(), 2);
                    EXPECT_NEAR(designPoint->Beta, 1.8250068211, 1e-4);
                    EXPECT_NEAR(designPoint->Alphas[0]->X, 1.27313, 1e-4);
                    EXPECT_NEAR(designPoint->Alphas[1]->X, -1.30758, 1e-4);
                }
            }

            void testReliabilityMethods::testNumericalBisection()
            {
                constexpr double margin = 1e-9;
                auto calculator = NumericalBisection();
                calculator.Settings->MaximumIterations = 20;
                calculator.Settings->designPointMethod = NearestToMean;

                auto modelRunner = projectBuilder().BuildProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                ASSERT_NEAR(designPoint->Beta, 1.87406654375, margin);
                EXPECT_NEAR(designPoint->Alphas[0]->X, 1.4371, 1e-4);
                EXPECT_NEAR(designPoint->Alphas[1]->X, -1.20282, 1e-4);
            }

            void testReliabilityMethods::testNumericalBisectionLinear()
            {
                auto calculator = std::make_shared<NumericalBisection>();

                auto project = projectBuilder::getLinearProject();
                project->reliabilityMethod = calculator;
                project->settings->ReliabilityMethod = ReliabilityNumericalBisection;
                project->settings->MaximumIterations = 20;
                project->settings->designPointMethod = CenterOfGravity;

                auto designPoint = project->getDesignPoint();

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                ASSERT_NEAR(designPoint->Beta, 2.57, 0.01);
                EXPECT_NEAR(designPoint->Alphas[0]->X, 0.931456, 1e-4);
                EXPECT_NEAR(designPoint->Alphas[1]->X, 0.931459, 1e-4);
            }

            void testReliabilityMethods::testCobylaReliability()
            {
                auto calculator = CobylaReliability();

                auto modelRunner = projectBuilder().BuildProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                EXPECT_NEAR(designPoint->Beta, 1.8741, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[0]->Alpha, -0.78693, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[1]->Alpha, 0.61704, 1e-3);
                EXPECT_TRUE(designPoint->convergenceReport->IsConverged);
            }

            void testReliabilityMethods::testSubSetSimulationReliabilityNearestToMean()
            {
                auto calculator = SubsetSimulation();
                calculator.Settings->SampleMethod = AdaptiveConditional;
                calculator.Settings->designPointMethod = NearestToMean;

                auto modelRunner = projectBuilder().BuildProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                EXPECT_NEAR(designPoint->Beta, 1.880647, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[0]->Alpha, -0.80691, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[1]->Alpha, 0.590669, 1e-3);
                EXPECT_FALSE(designPoint->convergenceReport->IsConverged);
            }

            void testReliabilityMethods::testSubSetSimulationReliabilityCenterOfGravity()
            {
                auto calculator = SubsetSimulation();
                calculator.Settings->SampleMethod = AdaptiveConditional;
                calculator.Settings->designPointMethod = CenterOfGravity;

                auto modelRunner = projectBuilder().BuildProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                EXPECT_NEAR(designPoint->Beta, 1.880647, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[0]->Alpha, -0.77594, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[1]->Alpha, 0.63081, 1e-3);
                EXPECT_FALSE(designPoint->convergenceReport->IsConverged);
            }

            void testReliabilityMethods::testFDIRReliability()
            {
                auto calculator = FORMThenDirectionalSampling();

                auto modelRunner = projectBuilder().BuildProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                EXPECT_NEAR(designPoint->Beta, 1.8741, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[0]->Alpha, -0.78087, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[1]->Alpha, 0.6247, 1e-3);
                EXPECT_TRUE(designPoint->convergenceReport->IsConverged);
            }

            void testReliabilityMethods::testDSFIReliability()
            {
                auto calculator = DirectionalSamplingThenFORM();

                auto modelRunner = projectBuilder().BuildProject();

                auto designPoint = calculator.getDesignPoint(modelRunner);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                EXPECT_NEAR(designPoint->Beta, 1.9117, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[0]->Alpha, -0.7809, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[1]->Alpha, 0.6247, 1e-3);
                EXPECT_TRUE(designPoint->convergenceReport->IsConverged);
            }

            void testReliabilityMethods::testFragilityCurveIntegration()
            {
                auto calculator = FragilityCurveIntegration();

                auto fragilityCurve = projectBuilder().BuildFragilityCurve();

                std::shared_ptr<Stochast> h = std::make_shared<Stochast>(DistributionType::Normal, std::vector{ 5.0, 1.0 });

                auto designPoint = calculator.getDesignPoint(h, fragilityCurve);

                ASSERT_EQ(designPoint->Alphas.size(), 2);
                EXPECT_NEAR(designPoint->Beta, 3.3572, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[0]->Alpha, -0.1578, 1e-3);
                EXPECT_NEAR(designPoint->Alphas[1]->Alpha, -0.9874, 1e-3);
                EXPECT_TRUE(designPoint->convergenceReport->IsConverged);
            }

            void testReliabilityMethods::testNumericalIntegrationReliability()
            {
                // test to see how num int handles sign for u==0:
                auto testValuesOffset = { -1e-100, 0.0, 1e-100 };

                for (const auto& testValue : testValuesOffset)
                {
                    auto calculator = NumericalIntegration();

                    auto modelRunner = projectBuilder().BuildProjectWithDeterminist(testValue);

                    auto designPoint = calculator.getDesignPoint(modelRunner);
                    const double sign = testValue > 0.0 ? 1.0 : -1.0;

                    ASSERT_EQ(designPoint->Alphas.size(), 3);
                    EXPECT_NEAR(designPoint->Beta, 0.0, 1e-6);
                    EXPECT_NEAR(designPoint->Alphas[0]->Alpha, sign * -0.624695, 1e-4);
                    EXPECT_NEAR(designPoint->Alphas[1]->Alpha, 0.0, 1e-6);
                    EXPECT_NEAR(designPoint->Alphas[2]->Alpha, sign * 0.78087, 1e-4);
                }
            }

            void testReliabilityMethods::testCrudeMonteCarloReliability()
            {
                auto calculator = CrudeMonteCarlo();
                auto modelRunner = projectBuilder().BuildProjectWithDeterminist(0.0);
                calculator.Settings->MinimumSamples = 10000;
                calculator.Settings->MaximumSamples = 100000;
                calculator.Settings->randomSettings->RandomGeneratorType = Numeric::RandomValueGeneratorType::MersenneTwister;
                auto designPoint = calculator.getDesignPoint(modelRunner);
                ASSERT_EQ(designPoint->Alphas.size(), 3);
                EXPECT_NEAR(designPoint->Beta, -0.01153, 1e-5);
            }

            void testReliabilityMethods::testClustersAdpImpSampling()
            {
                auto expectedBetas = std::vector<double>({ 0.80438, 0.753699, 0.78369, 0.7956208, 0.754192 });
                auto expectedCentersA = std::vector<double>( { -0.697351, 0.71673, -0.69555, -0.718478, 0.707038, -0.707175, 0.714029, 0.700116 });
                auto expectedCentersB = std::vector<double>({ -0.716727, 0.697354, 0.718175, 0.695863, -0.712197, -0.70198, 0.714321, -0.699818 });
                auto expectedCentersC = std::vector<double>({ -0.714813, 0.699316, -0.678015, -0.735048, 0.707866, 0.706347, 0.696578, -0.717481 });
                auto expectedCentersD = std::vector<double>({ -0.70989, -0.704313, 0.708422, 0.705789, -0.693545, 0.720413, 0.688054, -0.72566 });
                auto expectedCentersE = std::vector<double>({ -0.705994, -0.708218, 0.694748, -0.719253, -0.684285, 0.729214, 0.68589, 0.727705 });
                auto expectedCenters = std::vector({ expectedCentersA, expectedCentersB, expectedCentersC, expectedCentersD, expectedCentersE });

                for (int seed = 0; seed < 5; seed++)
                {
                    auto calculator = AdaptiveImportanceSampling();
                    auto modelRunner = projectBuilder().BuildQuadraticProject();
                    calculator.Settings->importanceSamplingSettings->MinimumSamples = 5000;
                    calculator.Settings->importanceSamplingSettings->MaximumSamples = 10000;
                    calculator.Settings->MinVarianceLoops = 2;
                    calculator.Settings->MaxVarianceLoops = 8;
                    calculator.Settings->importanceSamplingSettings->randomSettings->Seed = seed;
                    calculator.Settings->importanceSamplingSettings->runSettings->MaxParallelProcesses = 1;
                    calculator.Settings->Clustering = true;
                    calculator.Settings->clusterSettings->MaxClusters = 4;
                    calculator.Settings->clusterSettings->clusterInitializationMethod = Optimization::ClusterInitializationMethod::PlusPlus;
                    auto designPoint = calculator.getDesignPoint(modelRunner);
                    //std::cout << "Beta = " << designPoint->Beta << std::endl;
                    auto ii = 0;

                    for (int i = 0; i < 4; i++)
                    {
                        auto alpha = designPoint->ContributingDesignPoints[i]->Alphas;
                        auto expectedAlpha0 = expectedCenters[seed][ii];
                        ii++;
                        auto expectedAlpha1 = expectedCenters[seed][ii];
                        ii++;

                        EXPECT_NEAR(alpha[0]->Alpha, expectedAlpha0, 1e-4);
                        EXPECT_NEAR(alpha[1]->Alpha, expectedAlpha1, 1e-4);
                        //std::cout << ", " << alpha[0]->Alpha << ", " << alpha[1]->Alpha;
                    }
                    //std::cout << std::endl;
                    ASSERT_EQ(designPoint->Alphas.size(), 2);
                    EXPECT_NEAR(designPoint->Beta, expectedBetas[seed], 1e-4);
                }
            }
        }
    }
}
