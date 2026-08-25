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
#include "HandlerAdmin.h"
#include "../ProjectEntries.h"

namespace Deltares::Server
{
    HandlerAdmin::HandlerAdmin()
    {
        validationReportHandler.messageHandler = &messageHandler;
        scenarioHandler.stochastHandler = &stochastHandler;

        stochastHandler.validationReportHandler = &validationReportHandler;
        stochastHandler.discreteValueHandler = &discreteValueHandler;
        stochastHandler.histogramValueHandler = &histogramValueHandler;
        stochastHandler.fragilityValueHandler = &fragilityValueHandler;
        stochastHandler.conditionalValueHandler = &conditionalValueHandler;
        stochastHandler.contributingStochastHandler = &contributingStochastHandler;

        contributingStochastHandler.stochastHandler = &stochastHandler;

        handlers[ObjectType::HistogramValue] = &histogramValueHandler;
        handlers[ObjectType::DiscreteValue] = &discreteValueHandler;
        handlers[ObjectType::FragilityValue] = &fragilityValueHandler;
        handlers[ObjectType::Message] = &messageHandler;
        handlers[ObjectType::Evaluation] = &evaluationHandler;
        handlers[ObjectType::ProbabilityValue] = &probabilityValueHandler;
        handlers[ObjectType::StandardNormal] = &standardNormalHandler;
        handlers[ObjectType::Stochast] = &stochastHandler;
        handlers[ObjectType::Scenario] = &scenarioHandler;
        handlers[ObjectType::ValidationReport] = &validationReportHandler;
        handlers[ObjectType::ModelParameter] = &modelParameterHandler;
        handlers[ObjectType::LimitStateFunction] = &limitStateFunctionHandler;
        handlers[ObjectType::ReliabilityResult] = &reliabilityResultHandler;
        handlers[ObjectType::ConvergenceReport] = &convergenceReportHandler;
        handlers[ObjectType::ConditionalValue] = &conditionalValueHandler;
        handlers[ObjectType::ContributingStochast] = &contributingStochastHandler;

        for (const auto& [objectType, handler] : handlers)
        {
            handler->SetAdmin(&this->admin);
        }
    }

    bool HandlerAdmin::IsSupported(ObjectType objectType)
    {
        return
            objectType == ObjectType::HistogramValue ||
            objectType == ObjectType::DiscreteValue ||
            objectType == ObjectType::FragilityValue ||
            objectType == ObjectType::Message ||
            objectType == ObjectType::Evaluation ||
            objectType == ObjectType::ProbabilityValue ||
            objectType == ObjectType::StandardNormal ||
            objectType == ObjectType::Stochast ||
            objectType == ObjectType::Scenario ||
            objectType == ObjectType::ValidationReport ||
            objectType == ObjectType::ModelParameter ||
            objectType == ObjectType::LimitStateFunction ||
            objectType == ObjectType::ReliabilityResult ||
            objectType == ObjectType::ConvergenceReport ||
            objectType == ObjectType::ContributingStochast ||
            objectType == ObjectType::ConditionalValue;
    }
}

