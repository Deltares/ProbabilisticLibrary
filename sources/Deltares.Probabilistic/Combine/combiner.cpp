#include <unordered_set>

#include "combiner.h"

namespace Deltares
{
    namespace Reliability
    {
        std::vector<std::shared_ptr<Statistics::Stochast>> Combiner::getUniqueStochasts(std::vector<std::shared_ptr<DesignPoint>>& designPoints)
        {
            std::vector<std::shared_ptr<Statistics::Stochast>> uniqueStochasts;

            std::unordered_set<std::shared_ptr<Statistics::Stochast>> addedStochasts;

            // when multiple identical stochasts are present in one design point, they should all be added to the unique design points

            for (const std::shared_ptr<DesignPoint> designPoint : designPoints)
            {
                for (std::shared_ptr<StochastPointAlpha> alpha : designPoint->Alphas)
                {
                    if (!addedStochasts.contains(alpha->Stochast))
                    {
                        uniqueStochasts.push_back(alpha->Stochast);
                    }
                }

                for (std::shared_ptr<StochastPointAlpha> alpha : designPoint->Alphas)
                {
                    if (!addedStochasts.contains(alpha->Stochast))
                    {
                        addedStochasts.insert(alpha->Stochast);
                    }
                }
            }

            return uniqueStochasts;
        }



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


        std::shared_ptr<Project> Combiner::getProject(std::shared_ptr<Reliability::CombinedDesignPointModel> model, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix)
        {
            // create project
            std::shared_ptr<Project> project = std::make_shared<Project>();

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

