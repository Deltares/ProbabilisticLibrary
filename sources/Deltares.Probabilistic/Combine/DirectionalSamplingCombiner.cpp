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
#include "DirectionalSamplingCombiner.h"
#include "../Reliability/DirectionalSampling.h"
#include "../Reliability/DirectionalSamplingSettings.h"
#include "../Reliability/ReliabilityProject.h"

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Statistics;
        using namespace Deltares::Reliability;

        std::shared_ptr<DesignPoint> DirectionalSamplingCombiner::combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress)
        {
            std::vector<std::shared_ptr<Stochast>> stochasts = getUniqueStochasts(designPoints);

            // create the model from design points
            const std::shared_ptr<CombinedDesignPointModel> model = getModel(combineMethodType, nullptr, designPoints, stochasts, selfCorrelationMatrix);

            const std::shared_ptr<Reliability::ReliabilityProject> project = getProject(model, selfCorrelationMatrix);
            project->limitStateFunction->isActive = false;

            const std::shared_ptr<DirectionalSampling> directionalSampling = std::make_shared<DirectionalSampling>();
            fillSettings(model, directionalSampling->Settings);

            project->reliabilityMethod = directionalSampling;

            std::shared_ptr<DesignPoint> designPoint = project->getDesignPoint();

            // convert the stochasts back
            model->replaceStandardNormalStochasts(designPoint);

            for (size_t i = 0; i < designPoints.size(); i++)
            {
                designPoint->ContributingDesignPoints.push_back(designPoints[i]);
            }

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
            settings->randomSettings->RandomGeneratorType = randomGeneratorType;

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

