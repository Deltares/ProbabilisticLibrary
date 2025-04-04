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
#include "TestSensitivity.h"

#include "../projectBuilder.h"

#include "../../Deltares.Probabilistic/Sensitivity/SensitivityProject.h"
#include "../../Deltares.Probabilistic/Sensitivity/CrudeMonteCarloS.h"
#include "../../Deltares.Probabilistic/Sensitivity/ImportanceSamplingS.h"
#include "../../Deltares.Probabilistic/Sensitivity/NumericalIntegrationS.h"
#include "../../Deltares.Probabilistic/Sensitivity/DirectionalSamplingS.h"
#include "../../Deltares.Probabilistic/Sensitivity/FORMS.h"
#include "../../Deltares.Probabilistic/Sensitivity/FOSM.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void TestSensitivity::allSensitivityTests()
            {
                testCrudeMonteCarloAddOne();
                testCrudeMonteCarloLinear();
                testCrudeMonteCarloLinearNonRepeatable();
                testCrudeMonteCarloLinearOutput();
                testCrudeMonteCarloLinearOutputNonRepeatable();
                testCrudeMonteCarloLinearManySamples();
                testCrudeMonteCarloLinearAutoSamples();

                testImportanceSamplingAddOne();

                testNumericalIntegration();
                testDirectionalSampling();
                testFORM();
                testFOSM();
            }

            void TestSensitivity::testCrudeMonteCarloAddOne()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getAddOneProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::MersenneTwister;

                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getSensitivityResult().stochast;

                ASSERT_NEAR(stochast->getMean(), 1.0, margin);

                stochast->setDistributionType(Statistics::DistributionType::Uniform);

                ASSERT_NEAR(0.0, stochast->getProperties()->Minimum, margin);
                ASSERT_NEAR(2.0, stochast->getProperties()->Maximum, margin);
            }

            void TestSensitivity::testCrudeMonteCarloLinear()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::MersenneTwister;
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.05));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.5));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.95));

                project->sensitivityMethod = sensitivityMethod;

                auto result = project->getSensitivityResult();

                ASSERT_NEAR(1.8, result.stochast->getMean(), margin);
                ASSERT_NEAR(0.82, result.stochast->getDeviation(), margin);

                std::shared_ptr<Models::Evaluation> evaluation05 = result.quantileEvaluations[0];
                std::shared_ptr<Models::Evaluation> evaluation50 = result.quantileEvaluations[1];
                std::shared_ptr<Models::Evaluation> evaluation95 = result.quantileEvaluations[2];

                ASSERT_NEAR(0.42, evaluation05->Z, margin);
                ASSERT_NEAR(1.78, evaluation50->Z, margin);
                ASSERT_NEAR(0.00, evaluation50->InputValues[0] + evaluation50->InputValues[1], margin);
                ASSERT_NEAR(3.18, evaluation95->Z, margin);
            }

            void TestSensitivity::testCrudeMonteCarloLinearNonRepeatable()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::MersenneTwister;
                sensitivityMethod->Settings->randomSettings->IsRepeatableRandom = false;
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.05));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.10));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.90));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.95));

                project->sensitivityMethod = sensitivityMethod;

                auto result = project->getSensitivityResult();

                ASSERT_LE(result.quantileEvaluations[0]->Z, 1.5);
                ASSERT_LE(result.quantileEvaluations[1]->Z, 1.5);
                ASSERT_GE(result.quantileEvaluations[2]->Z, 2.1);
                ASSERT_GE(result.quantileEvaluations[3]->Z, 2.1);

                // do it again, check results are not equal

                auto result2 = project->getSensitivityResult();

                const double smallMargin = 1E-10;

                ASSERT_NE(result.stochast, result2.stochast);

                double m1 = result.stochast->getMean();
                double m2 = result2.stochast->getMean();

                ASSERT_FALSE(std::abs(m1 - m2) < smallMargin);
            }

            void TestSensitivity::testCrudeMonteCarloLinearOutput()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearOutputProject());

                project->settings->SensitivityMethod = Sensitivity::SensitivityCrudeMonteCarlo;
                project->settings->RandomSettings->IsRepeatableRandom = true;
                project->settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.95));

                project->run();

                ASSERT_EQ(2, project->sensitivityResults.size());

                const double smallMargin = 1E-10;

                ASSERT_NEAR(project->sensitivityResults[0]->stochast->getMean(), project->sensitivityResults[1]->stochast->getMean(), smallMargin);
                ASSERT_NEAR(project->sensitivityResults[0]->stochast->getDeviation(), project->sensitivityResults[1]->stochast->getDeviation(), smallMargin);
                ASSERT_NEAR(project->sensitivityResults[0]->quantileEvaluations[0]->OutputValues[0], project->sensitivityResults[1]->quantileEvaluations[0]->OutputValues[0], smallMargin);
            }

            void TestSensitivity::testCrudeMonteCarloLinearOutputNonRepeatable()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearOutputProject());

                project->settings->SensitivityMethod = Sensitivity::SensitivityCrudeMonteCarlo;
                project->settings->RandomSettings->IsRepeatableRandom = false;
                project->settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.95));

                project->run();

                ASSERT_EQ(2, project->sensitivityResults.size());

                const double smallMargin = 1E-10;

                ASSERT_NEAR(project->sensitivityResults[0]->stochast->getMean(), project->sensitivityResults[1]->stochast->getMean(), smallMargin);
                ASSERT_NEAR(project->sensitivityResults[0]->stochast->getDeviation(), project->sensitivityResults[1]->stochast->getDeviation(), smallMargin);
                ASSERT_NEAR(project->sensitivityResults[0]->quantileEvaluations[0]->OutputValues[0], project->sensitivityResults[1]->quantileEvaluations[0]->OutputValues[0], smallMargin);
            }

            void TestSensitivity::testCrudeMonteCarloLinearManySamples()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::MersenneTwister;
                sensitivityMethod->Settings->MaximumSamples = 100000;

                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getSensitivityResult().stochast;

                ASSERT_NEAR(1.8, stochast->getMean(), margin);
                ASSERT_NEAR(0.82, stochast->getDeviation(), margin);

                stochast->setDistributionType(Statistics::DistributionType::Uniform);

                ASSERT_NEAR(-0.2, stochast->getProperties()->Minimum, 10 * margin);
                ASSERT_NEAR(3.8, stochast->getProperties()->Maximum, 10 * margin);
            }

            void TestSensitivity::testCrudeMonteCarloLinearAutoSamples()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::MersenneTwister;
                sensitivityMethod->Settings->DeriveSamplesFromVariationCoefficient = true;

                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getSensitivityResult().stochast;

                ASSERT_EQ(7600, sensitivityMethod->Settings->getRequiredSamples());
                ASSERT_NEAR(1.8, stochast->getMean(), margin);
                ASSERT_NEAR(0.82, stochast->getDeviation(), margin);
            }

            void TestSensitivity::testImportanceSamplingAddOne()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getAddOneProject());

                std::shared_ptr<Sensitivity::ImportanceSamplingS> sensitivityMethod = std::make_shared<Sensitivity::ImportanceSamplingS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::MersenneTwister;
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.5));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.95));

                project->sensitivityMethod = sensitivityMethod;

                auto result = project->getSensitivityResult();

                ASSERT_NEAR(result.stochast->getMean(), 1.0, margin);

                result.stochast->setDistributionType(Statistics::DistributionType::Uniform);

                ASSERT_NEAR(0.0, result.stochast->getProperties()->Minimum, margin);
                ASSERT_NEAR(2.0, result.stochast->getProperties()->Maximum, margin);

                ASSERT_NEAR(1.00, result.quantileEvaluations[0]->Z, margin);
                ASSERT_NEAR(1.90, result.quantileEvaluations[1]->Z, margin);
            }

            void TestSensitivity::testNumericalIntegration()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::NumericalIntegrationS> sensitivityMethod = std::make_shared<Sensitivity::NumericalIntegrationS>();
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.05));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.5));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.95));
                project->sensitivityMethod = sensitivityMethod;

                auto result = project->getSensitivityResult();

                ASSERT_NEAR(result.stochast->getMean(), 1.8, margin);
                ASSERT_NEAR(result.stochast->getDeviation(), 0.81, margin);

                ASSERT_NEAR(0.43, result.quantileEvaluations[0]->Z, margin);
                ASSERT_NEAR(1.80, result.quantileEvaluations[1]->Z, margin);
                ASSERT_NEAR(3.17, result.quantileEvaluations[2]->Z, margin);
            }

            void TestSensitivity::testDirectionalSampling()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::DirectionalSamplingS> sensitivityMethod = std::make_shared<Sensitivity::DirectionalSamplingS>();

                std::shared_ptr<Statistics::ProbabilityValue> value1 = std::make_shared<Statistics::ProbabilityValue>();
                value1->setProbabilityOfNonFailure(0.9);

                sensitivityMethod->Settings->RequestedQuantiles.push_back(value1);
                project->sensitivityMethod = sensitivityMethod;

                auto result = project->getSensitivityResult();

                ASSERT_NEAR(result.stochast->getProperties()->FragilityValues[0]->X, 2.9, margin);
                ASSERT_NEAR(result.stochast->getProperties()->FragilityValues[0]->getProbabilityOfNonFailure(), 0.9, margin);

                ASSERT_NEAR(2.91, result.quantileEvaluations[0]->Z, margin);
            }

            void TestSensitivity::testFORM()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::FORMS> sensitivityMethod = std::make_shared<Sensitivity::FORMS>();
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.05));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.5));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.95));
                project->sensitivityMethod = sensitivityMethod;

                auto result = project->getSensitivityResult();

                ASSERT_NEAR(result.stochast->getMean(), 1.8, margin);
                ASSERT_NEAR(result.stochast->getDeviation(), 0.92, margin);

                ASSERT_NEAR(0.30, result.quantileEvaluations[0]->Z, margin);
                ASSERT_NEAR(1.80, result.quantileEvaluations[1]->Z, margin);
                ASSERT_NEAR(3.30, result.quantileEvaluations[2]->Z, margin);
            }

            void TestSensitivity::testFOSM()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::FOSM> sensitivityMethod = std::make_shared<Sensitivity::FOSM>();
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.05));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.5));
                sensitivityMethod->Settings->RequestedQuantiles.push_back(std::make_shared<Statistics::ProbabilityValue>(0.95));
                project->sensitivityMethod = sensitivityMethod;

                auto result = project->getSensitivityResult();

                ASSERT_NEAR(result.stochast->getMean(), 1.8, margin);
                ASSERT_NEAR(result.stochast->getDeviation(), 1.04, margin);

                ASSERT_NEAR(0.29, result.quantileEvaluations[0]->Z, margin);
                ASSERT_NEAR(1.80, result.quantileEvaluations[1]->Z, margin);
                ASSERT_NEAR(3.31, result.quantileEvaluations[2]->Z, margin);
            }
        }
    }
}


