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
        std::shared_ptr<Models::RunProject> project = std::make_shared<Models::RunProject>();

        project->settings->runValuesType = Statistics::DesignValues;

        ASSERT_EQ(false, project->isValid());

        project->run();

        ASSERT_EQ(nullptr, project->evaluation);
    }

}


