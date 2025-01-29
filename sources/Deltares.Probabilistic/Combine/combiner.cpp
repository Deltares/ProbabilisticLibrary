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
#include <unordered_set>

#include "combiner.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<Reliability::CombinedDesignPointModel> Combiner::getModel(combineAndOr combineMethodType, std::shared_ptr<Reliability::DesignPoint> currentDesignPoint, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix)
        {
            // create the model from design points
            std::shared_ptr<Reliability::CombinedDesignPointModel> model = std::make_shared<CombinedDesignPointModel>();
            model->combineType = combineMethodType;

            if (currentDesignPoint != nullptr)
            {
                model->addDesignPointModel(currentDesignPoint, true, true);
            }

            for (std::shared_ptr<DesignPoint> designPoint : previousDesignPoints)
            {
                model->addDesignPointModel(designPoint);
            }

            model->addStochasts(stochasts, selfCorrelationMatrix);

            return model;
        }


        std::shared_ptr<ReliabilityProject> Combiner::getProject(std::shared_ptr<Reliability::CombinedDesignPointModel> model, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix)
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
            for (size_t i = 0; i < model->standardNormalStochasts.size(); i++)
            {
                project->stochasts.push_back(model->standardNormalStochasts[i]);
            }

            project->correlationMatrix = model->getCorrelationMatrix(selfCorrelationMatrix);

            ZLambda zFunction = [model](std::shared_ptr<ModelSample> sample)
            {
                model->calculate(sample);
            };

            std::shared_ptr<ZModel> zModel = std::make_shared<ZModel>(zFunction);

            if (model->canCalculateBetaDirection())
            {
                ZBetaLambda zBetaFunction = [model](std::shared_ptr<ModelSample> sample, double beta)
                {
                    return model->getBetaDirection(sample);
                };

                zModel->setBetaLambda(zBetaFunction);
            }

            project->model = zModel;

            return project;
        }
    }
}

