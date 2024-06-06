#include "DirectionalSamplingCombiner.h"
#include "../Reliability/DirectionalSampling.h"
#include "../Reliability/DirectionalSamplingSettings.h"
#include "../Model/Project.h"

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Statistics;
        using namespace Deltares::Reliability;

        std::shared_ptr<DesignPoint> DirectionalSamplingCombiner::CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints, const std::vector<double>& rho, const combineAndOr cmbType)
        {
            return nullptr;
        }

        std::shared_ptr<DesignPoint> DirectionalSamplingCombiner::combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress)
        {
            std::vector<std::shared_ptr<Stochast>> stochasts = getUniqueStochasts(designPoints);

            // create the model from design points
            const std::shared_ptr<CombinedDesignPointModel> model = getModel(combineMethodType, nullptr, designPoints, stochasts, selfCorrelationMatrix);

            const std::shared_ptr<Project> project = getProject(model, correlationMatrix, selfCorrelationMatrix);

            const std::shared_ptr<DirectionalSampling> directionalSampling = std::make_shared<DirectionalSampling>();
            fillSettings(model, directionalSampling->Settings);

            project->reliabilityMethod = directionalSampling;

            std::shared_ptr<DesignPoint> designPoint = project->getDesignPoint();

            // convert the stochasts back
            model->replaceStandardNormalStochasts(designPoint);

            return designPoint;
        }

        void DirectionalSamplingCombiner::fillSettings(std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<DirectionalSamplingSettings> settings)
        {
            settings->MinimumDirections = 1000;
            settings->MaximumDirections = 100000;
            settings->DirectionSettings->MaximumLengthU = 20; // not used
            settings->DirectionSettings->Dsdu = 0.1;
            settings->VariationCoefficient = 0.1;
            settings->DirectionSettings->modelVaryingType = ModelVaryingType::Varying;
            settings->designPointMethod = DesignPointMethod::CenterOfGravity;
            settings->runSettings->MaxChunkSize = 400;
            settings->randomSettings->SkipUnvaryingParameters = false;

            for (size_t i = 0; i < model->stochasts.size(); i++)
            {
                std::shared_ptr<StochastSettings> stochastSetting = std::make_shared<StochastSettings>();
                stochastSetting->stochast = model->standardNormalStochasts[i];
                stochastSetting->StartValue = 0;
                stochastSetting->VarianceFactor = 1;
                settings->StochastSet->stochastSettings.push_back(stochastSetting);
            }
        }
    }
}

