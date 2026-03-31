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

#include "Combiner.h"

namespace Deltares::Reliability
{
    std::shared_ptr<CombinedDesignPointModel> Combiner::getModel(combineAndOr combineMethodType,
        const std::shared_ptr<DesignPoint>& currentDesignPoint,
        const std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints,
        const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts,
        const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix)
    {
        // create the model from design points
        auto model = std::make_shared<CombinedDesignPointModel>();
        model->combineType = combineMethodType;

        if (currentDesignPoint != nullptr)
        {
            model->addDesignPointModel(currentDesignPoint, true, true);
        }

        for (const std::shared_ptr<DesignPoint>& designPoint : previousDesignPoints)
        {
            model->addDesignPointModel(designPoint);
        }

        model->addStochasts(stochasts, selfCorrelationMatrix);

        return model;
    }


    std::shared_ptr<ReliabilityProject> Combiner::getProject(const std::shared_ptr<CombinedDesignPointModel>& model,
        const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix)
    {
        // create project
        std::shared_ptr<ReliabilityProject> project = std::make_shared<ReliabilityProject>();

        // set run settings with minimal overhead
        project->runSettings->SaveMessages = false;
        project->runSettings->SaveEvaluations = false;
        project->runSettings->SaveConvergence = true;
        project->runSettings->MaxParallelProcesses = 1;
        project->runSettings->MaxChunkSize = 1000;

        // register all stochasts in the project
        for (const auto& standardNormalStochast : model->standardNormalStochasts)
        {
            project->stochasts.push_back(standardNormalStochast);
        }

        project->correlation = model->getCorrelationMatrix(selfCorrelationMatrix);

        Models::ZLambda zFunction = [model](const std::shared_ptr<Models::ModelSample>& sample)
        {
            model->calculate(sample);
        };

        const auto zModel = std::make_shared<Models::ZModel>(zFunction);

        if (model->canCalculateBetaDirection())
        {
            Models::ZBetaLambda zBetaFunction = [model](const std::shared_ptr<Models::ModelSample>& sample)
            {
                return model->getBetaDirection(sample);
            };

            zModel->setBetaLambda(zBetaFunction);
        }

        project->model = zModel;

        return project;
    }
}

