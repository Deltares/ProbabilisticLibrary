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
#include "StatisticsHandlers.h"

namespace Deltares::Server
{
    void StatisticsHandlers::InitializeHandlers(std::unordered_map<ObjectType, ObjectHandler*>& handlers, ModelHandlers* modelHandlers)
    {
        stochastHandler.validationReportHandler = &modelHandlers->validationReportHandler;
        stochastHandler.discreteValueHandler = &discreteValueHandler;
        stochastHandler.histogramValueHandler = &histogramValueHandler;
        stochastHandler.fragilityValueHandler = &fragilityValueHandler;
        stochastHandler.conditionalValueHandler = &conditionalValueHandler;
        stochastHandler.contributingStochastHandler = &contributingStochastHandler;

        correlationMatrixHandler.stochastHandler = &stochastHandler;
        copulaCorrelationHandler.stochastHandler = &stochastHandler;
        selfCorrelationMatrixHandler.stochastHandler = &stochastHandler;

        contributingStochastHandler.stochastCallback = [this](const int id) {return this->stochastHandler.GetObject(id); };
        contributingStochastHandler.stochastIdCallback = [this](const std::shared_ptr<Statistics::Stochast>& stochast) {return this->stochastHandler.GetObjectId(stochast); };

        scenarioHandler.stochastHandler = &stochastHandler;

        alphaHandler.stochastHandler = &stochastHandler;

        stochastPointHandler.alphaHandler = &alphaHandler;

        stochastSettingsHandler.stochastHandler = &stochastHandler;

        modelProjectSettingsHandler.validationReportHandler = &modelHandlers->validationReportHandler;

        runProjectSettingsHandler.modelProjectSettingsHandler = &modelProjectSettingsHandler;

        modelProjectHandler.validationReportHandler = &modelHandlers->validationReportHandler;
        modelProjectHandler.modelParameterHandler = &modelHandlers->modelParameterHandler;
        modelProjectHandler.stochastHandler = &stochastHandler;
        modelProjectHandler.correlationMatrixHandler = &correlationMatrixHandler;
        modelProjectHandler.copulaCorrelationHandler = &copulaCorrelationHandler;

        runProjectHandler.modelProjectHandler = &modelProjectHandler;
        runProjectHandler.runProjectSettingsHandler = &runProjectSettingsHandler;
        runProjectHandler.evaluationHandler = &modelHandlers->evaluationHandler;

        handlers[ObjectType::HistogramValue] = &histogramValueHandler;
        handlers[ObjectType::DiscreteValue] = &discreteValueHandler;
        handlers[ObjectType::FragilityValue] = &fragilityValueHandler;
        handlers[ObjectType::ProbabilityValue] = &probabilityValueHandler;
        handlers[ObjectType::StandardNormal] = &standardNormalHandler;
        handlers[ObjectType::Stochast] = &stochastHandler;
        handlers[ObjectType::Scenario] = &scenarioHandler;
        handlers[ObjectType::ContributingStochast] = &contributingStochastHandler;
        handlers[ObjectType::CorrelationMatrix] = &correlationMatrixHandler;
        handlers[ObjectType::CopulaCorrelation] = &copulaCorrelationHandler;
        handlers[ObjectType::SelfCorrelationMatrix] = &selfCorrelationMatrixHandler;
        handlers[ObjectType::StochastSettings] = &stochastSettingsHandler;
        handlers[ObjectType::Alpha] = &alphaHandler;
        handlers[ObjectType::StochastPoint] = &stochastPointHandler;
        handlers[ObjectType::ConditionalValue] = &conditionalValueHandler;
        handlers[ObjectType::RunProjectSettings] = &runProjectSettingsHandler;
        handlers[ObjectType::RunProject] = &runProjectHandler;
    }
}

