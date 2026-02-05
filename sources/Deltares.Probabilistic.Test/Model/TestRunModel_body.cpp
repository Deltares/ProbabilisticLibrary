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
#include "TestRunModel.h"

#include "../projectBuilder.h"

#include "../../Deltares.Probabilistic/Model/RunProject.h"

namespace Deltares::Probabilistic::Test
{
    void TestRunModel::allModelTests() const
    {
        testRunModelMedianValues();
        testRunModelMeanValues();
        testRunModelDesignValues();
        testNoModel();
        testRunModelArraysVariable();
        testRunModelVariableWithArrayVariables();
    }

    void TestRunModel::testRunModelMedianValues() const
    {
        std::shared_ptr<Models::RunProject> project = projectBuilder::getRunProject(projectBuilder::getTriangularLinearProject());

        project->settings->runValuesType = Statistics::MedianValues;

        project->run();

        ASSERT_NEAR(project->evaluation->Z, 1.21, margin);
        ASSERT_NEAR(project->evaluation->InputValues[0], project->stochasts[0]->getXFromU(0), margin);
        ASSERT_NEAR(project->evaluation->InputValues[1], project->stochasts[1]->getXFromU(0), margin);
    }

    void TestRunModel::testRunModelMeanValues() const
    {
        std::shared_ptr<Models::RunProject> project = projectBuilder::getRunProject(projectBuilder::getTriangularLinearProject());

        project->settings->runValuesType = Statistics::MeanValues;

        project->run();

        ASSERT_NEAR(project->evaluation->Z, 1.13, margin);
        ASSERT_NEAR(project->evaluation->InputValues[0], project->stochasts[0]->getMean(), margin);
        ASSERT_NEAR(project->evaluation->InputValues[1], project->stochasts[1]->getMean(), margin);
    }

    void TestRunModel::testRunModelDesignValues() const
    {
        std::shared_ptr<Models::RunProject> project = projectBuilder::getRunProject(projectBuilder::getTriangularLinearProject());

        project->settings->runValuesType = Statistics::DesignValues;

        project->stochasts[0]->designQuantile = 0.95;
        project->stochasts[0]->designFactor = 1.5;

        double u = Statistics::StandardNormal::getUFromP(0.95);

        project->run();

        ASSERT_NEAR(project->evaluation->Z, 0.99, margin);
        ASSERT_NEAR(project->evaluation->InputValues[0], project->stochasts[0]->getXFromU(u) / 1.5, margin);
        ASSERT_NEAR(project->evaluation->InputValues[1], project->stochasts[1]->getXFromU(0), margin);
    }

    void TestRunModel::testNoModel() const
    {
        auto project = Models::RunProject();

        project.settings->runValuesType = Statistics::DesignValues;

        ASSERT_FALSE(project.isValid());

        project.run();

        ASSERT_EQ(nullptr, project.evaluation);
    }

    void TestRunModel::testRunModelArraysVariable() const
    {
        std::shared_ptr<Models::RunProject> project = projectBuilder::getRunProject(projectBuilder::getArrayVariableProject());

        project->settings->runValuesType = Statistics::MeanValues;

        ASSERT_TRUE(project->isValid());

        project->run();

        ASSERT_NEAR(project->evaluation->Z, 5.0, margin);
        ASSERT_EQ(project->evaluation->InputValues.size(), 10);

        // should succeed, the source array is greater than the target array
        project->stochasts[0]->modelParameter->arraySize = 6;
        ASSERT_TRUE(project->isValid());

        // should fail, the source array is less than the target array
        project->stochasts[1]->modelParameter->arraySize = 7;
        ASSERT_FALSE(project->isValid());
    }

    void TestRunModel::testRunModelVariableWithArrayVariables() const
    {
        std::shared_ptr<Models::RunProject> project = projectBuilder::getRunProject(projectBuilder::getArrayVariableProject());

        project->settings->runValuesType = Statistics::MeanValues;

        std::vector<double> values = { 0.8 };
        auto arrayStochast = std::make_shared<Statistics::Stochast>(Statistics::DistributionType::Deterministic, values);

        for (int i = 0; i < 3; i++)
        {
            project->stochasts[1]->ArrayVariables.push_back(arrayStochast);
        }

        ASSERT_TRUE(project->isValid());

        project->run();

        ASSERT_NEAR(project->evaluation->Z, 5.9, margin);
        ASSERT_EQ(project->evaluation->InputValues.size(), 10);

        // should fail, array variables are specified but target size is greater than source size
        project->stochasts[1]->modelParameter->arraySize = 6;
        ASSERT_FALSE(project->isValid());

        // should succeed, target size is greater than source size but array variables are used for the whole range of target variables
        for (int i = 3; i < 6; i++)
        {
            project->stochasts[1]->ArrayVariables.push_back(arrayStochast);
        }

        ASSERT_TRUE(project->isValid());

        project->run();

        ASSERT_NEAR(project->evaluation->Z, 7.3, margin);
        ASSERT_EQ(project->evaluation->InputValues.size(), 11);

        // should succeed, the array variables are variables stochasts 
        project->stochasts[0]->modelParameter->arraySize = 6; // make equal size again
        arrayStochast->IsVariableStochast = true;
        arrayStochast->VariableSource = project->stochasts[0];

        auto value1 = std::make_shared<Statistics::VariableStochastValue>();
        value1->X = 0.2;
        value1->Stochast->Minimum = 0.2;
        value1->Stochast->Maximum = 0.4;
        arrayStochast->ValueSet->StochastValues.push_back(value1);

        auto value2 = std::make_shared<Statistics::VariableStochastValue>();
        value2->X = 0.8;
        value2->Stochast->Minimum = 0.6;
        value2->Stochast->Maximum = 0.8;
        arrayStochast->ValueSet->StochastValues.push_back(value2);

        ASSERT_TRUE(project->isValid());

        // should fail, the last array stochast can not be matched to a source stochast
        project->stochasts[1]->modelParameter->arraySize = 7;
        ASSERT_FALSE(project->isValid());

        // should succeed, the last array stochast is connected to a new stochast
        auto arrayStochast2 = std::make_shared<Statistics::Stochast>(Statistics::DistributionType::Deterministic, values);
        project->stochasts[1]->ArrayVariables.push_back(arrayStochast2);
        ASSERT_TRUE(project->isValid());
    }

}


