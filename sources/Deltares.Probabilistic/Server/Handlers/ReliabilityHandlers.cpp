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
#include "ReliabilityHandlers.h"

#include <unordered_map>
#include "ObjectHandler.h"

namespace Deltares::Server
{
    void ReliabilityHandlers::InitializeHandlers(std::unordered_map<ObjectType, ObjectHandler*>& handlers, ModelHandlers* modelHandlers, StatisticsHandlers* statisticsHandlers)
    {
        fragilityCurveHandler.stochastHandler = &statisticsHandlers->stochastHandler;

        probabilityLimitStateFunctionHandler.fragilityCurveHandler = &fragilityCurveHandler;

        combinedLimitStateFunctionHandler.limitStateFunctionHandler = &limitStateFunctionHandler;

        designPointHandler.convergenceReportHandler = &convergenceReportHandler;
        designPointHandler.evaluationHandler = &modelHandlers->evaluationHandler;
        designPointHandler.reliabilityResultHandler = &reliabilityResultHandler;
        designPointHandler.messageHandler = &modelHandlers->messageHandler;
        designPointHandler.alphaHandler = &statisticsHandlers->alphaHandler;

        reliabilitySettingsHandler.modelProjectSettingsHandler = &statisticsHandlers->modelProjectSettingsHandler;
        reliabilitySettingsHandler.stochastSettingsHandler = &statisticsHandlers->stochastSettingsHandler; 
        reliabilitySettingsHandler.designPointCallback = [this](const int id) {return this->designPointHandler.GetObject(id); };

        combineProjectHandler.combineSettingsHandler = &combineSettingsHandler;
        combineProjectHandler.designPointHandler = &designPointHandler;
        combineProjectHandler.correlationMatrixHandler = &statisticsHandlers->correlationMatrixHandler;
        combineProjectHandler.copulaCorrelationHandler = &statisticsHandlers->copulaCorrelationHandler;
        combineProjectHandler.selfCorrelationMatrixHandler = &statisticsHandlers->selfCorrelationMatrixHandler;
        combineProjectHandler.validationReportHandler = &modelHandlers->validationReportHandler;

        excludingCombineProjectHandler.excludingCombineSettingsHandler = &excludingCombineSettingsHandler;
        excludingCombineProjectHandler.designPointHandler = &designPointHandler;
        excludingCombineProjectHandler.scenarioHandler = &statisticsHandlers->scenarioHandler;
        excludingCombineProjectHandler.validationReportHandler = &modelHandlers->validationReportHandler;

        lengthEffectProjectHandler.designPointHandler = &designPointHandler;
        lengthEffectProjectHandler.selfCorrelationMatrixHandler = &statisticsHandlers->selfCorrelationMatrixHandler;

        fragilityCurveProjectHandler.designPointHandler = &designPointHandler;
        fragilityCurveProjectHandler.stochastHandler = &statisticsHandlers->stochastHandler;
        fragilityCurveProjectHandler.fragilityCurveHandler = &fragilityCurveHandler;
        fragilityCurveProjectHandler.fragilityCurveSettingsHandler = &fragilityCurveSettingsHandler;

        reliabilityProjectHandler.modelProjectHandler = &statisticsHandlers->modelProjectHandler;
        reliabilityProjectHandler.reliabilitySettingsHandler = &reliabilitySettingsHandler;
        reliabilityProjectHandler.limitStateFunctionHandler = &limitStateFunctionHandler;
        reliabilityProjectHandler.designPointHandler = &designPointHandler;
        reliabilityProjectHandler.limitStateFunctionCallback = [this](int id) { return this->GetLimitStateFunction(id); };

        handlers[ObjectType::FragilityCurve] = &fragilityCurveHandler;
        handlers[ObjectType::LimitStateFunction] = &limitStateFunctionHandler;
        handlers[ObjectType::ProbabilityLimitStateFunction] = &probabilityLimitStateFunctionHandler;
        handlers[ObjectType::CombinedLimitStateFunction] = &combinedLimitStateFunctionHandler;
        handlers[ObjectType::ReliabilityResult] = &reliabilityResultHandler;
        handlers[ObjectType::ConvergenceReport] = &convergenceReportHandler;
        handlers[ObjectType::DesignPoint] = &designPointHandler;
        handlers[ObjectType::Settings] = &reliabilitySettingsHandler;
        handlers[ObjectType::FragilityCurveSettings] = &fragilityCurveSettingsHandler;
        handlers[ObjectType::CombineSettings] = &combineSettingsHandler;
        handlers[ObjectType::ExcludingCombineSettings] = &excludingCombineSettingsHandler;
        handlers[ObjectType::CombineProject] = &combineProjectHandler;
        handlers[ObjectType::ExcludingCombineProject] = &excludingCombineProjectHandler;
        handlers[ObjectType::LengthEffectProject] = &lengthEffectProjectHandler;
        handlers[ObjectType::FragilityCurveProject] = &fragilityCurveProjectHandler;
        handlers[ObjectType::Project] = &reliabilityProjectHandler;
    }

    std::shared_ptr<Reliability::LimitStateFunction> ReliabilityHandlers::GetLimitStateFunction(int id)
    {
        if (limitStateFunctionHandler.Contains(id))
        {
            return limitStateFunctionHandler.GetObject(id);
        }
        else if (combinedLimitStateFunctionHandler.Contains(id))
        {
            return combinedLimitStateFunctionHandler.GetObject(id);
        }
        else if (probabilityLimitStateFunctionHandler.Contains(id))
        {
            return probabilityLimitStateFunctionHandler.GetObject(id);
        }
        else
        {
            return nullptr;
        }
    }


}

