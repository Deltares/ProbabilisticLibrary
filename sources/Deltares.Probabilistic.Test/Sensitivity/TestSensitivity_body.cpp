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
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::MersenneTwister;

                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_NEAR(stochast->getMean(), 1.0, margin);

                stochast->setDistributionType(Statistics::DistributionType::Uniform);

                ASSERT_NEAR(0.0, stochast->getProperties()->Minimum, margin);
                ASSERT_NEAR(2.0, stochast->getProperties()->Maximum, margin);
            }

            void TestSensitivity::testCrudeMonteCarloLinear()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::MersenneTwister;

                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_NEAR(1.8, stochast->getMean(), margin);
                ASSERT_NEAR(0.82, stochast->getDeviation(), margin);
            }

            void TestSensitivity::testCrudeMonteCarloLinearManySamples()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::MersenneTwister;
                sensitivityMethod->Settings->MaximumSamples = 100000;

                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

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
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::MersenneTwister;
                sensitivityMethod->Settings->DeriveSamplesFromVariationCoefficient = true;

                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_EQ(7600, sensitivityMethod->Settings->getRequiredSamples());
                ASSERT_NEAR(1.8, stochast->getMean(), margin);
                ASSERT_NEAR(0.82, stochast->getDeviation(), margin);
            }

            void TestSensitivity::testImportanceSamplingAddOne()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getAddOneProject());

                std::shared_ptr<Sensitivity::ImportanceSamplingS> sensitivityMethod = std::make_shared<Sensitivity::ImportanceSamplingS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::MersenneTwister;

                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_NEAR(stochast->getMean(), 1.0, margin);

                stochast->setDistributionType(Statistics::DistributionType::Uniform);

                ASSERT_NEAR(0.0, stochast->getProperties()->Minimum, margin);
                ASSERT_NEAR(2.0, stochast->getProperties()->Maximum, margin);
            }

            void TestSensitivity::testNumericalIntegration()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::NumericalIntegrationS> sensitivityMethod = std::make_shared<Sensitivity::NumericalIntegrationS>();
                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_NEAR(stochast->getMean(), 1.8, margin);
                ASSERT_NEAR(stochast->getDeviation(), 0.81, margin);
            }

            void TestSensitivity::testDirectionalSampling()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::DirectionalSamplingS> sensitivityMethod = std::make_shared<Sensitivity::DirectionalSamplingS>();

                std::shared_ptr<Statistics::ProbabilityValue> value1 = std::make_shared<Statistics::ProbabilityValue>();
                value1->setProbabilityOfNonFailure(0.9);

                sensitivityMethod->Settings->RequestedQuantiles.push_back(value1);
                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_NEAR(stochast->getProperties()->FragilityValues[0]->X, 2.9, margin);
                ASSERT_NEAR(stochast->getProperties()->FragilityValues[0]->getProbabilityOfNonFailure(), 0.9, margin);
            }

            void TestSensitivity::testFORM()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::FORMS> sensitivityMethod = std::make_shared<Sensitivity::FORMS>();
                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_NEAR(stochast->getMean(), 1.76, margin);
                ASSERT_NEAR(stochast->getDeviation(), 1.02, margin);
            }

            void TestSensitivity::testFOSM()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::FOSM> sensitivityMethod = std::make_shared<Sensitivity::FOSM>();
                project->sensitivityMethod = sensitivityMethod;

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_NEAR(stochast->getMean(), 1.8, margin);
                ASSERT_NEAR(stochast->getDeviation(), 1.04, margin);
            }
        }
    }
}


