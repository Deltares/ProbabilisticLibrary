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

        contributingStochastHandler.stochastCallback = [this](int id) {return this->stochastHandler.GetObject(id); };
        contributingStochastHandler.stochastIdCallback = [this](std::shared_ptr<Statistics::Stochast> stochast) {return this->stochastHandler.GetObjectId(stochast); };

        fragilityCurveHandler.stochastHandler = &stochastHandler;

        fragilityValueHandler.designPointCallback = [this](int id) {return this->designPointHandler.GetObject(id); };
        fragilityValueHandler.designPointIdCallback = [this](std::shared_ptr<Reliability::DesignPoint> designPoint) {return this->designPointHandler.GetObjectId(designPoint); };

        alphaHandler.stochastHandler = &stochastHandler;
        alphaHandler.fragilityCurveHandler = &fragilityCurveHandler;

        designPointHandler.convergenceReportHandler = &convergenceReportHandler;
        designPointHandler.evaluationHandler = &evaluationHandler;
        designPointHandler.reliabilityResultHandler = &reliabilityResultHandler;
        designPointHandler.messageHandler = &messageHandler;
        designPointHandler.alphaHandler = &alphaHandler;

        stochastPointHandler.alphaHandler = &alphaHandler;

        probabilityLimitStateFunctionHandler.fragilityCurveHandler = &fragilityCurveHandler;

        combinedLimitStateFunctionHandler.limitStateFunctionHandler = &limitStateFunctionHandler;

        uncertaintyResultHandler.stochastHandler = &stochastHandler;
        uncertaintyResultHandler.evaluationHandler = &evaluationHandler;
        uncertaintyResultHandler.messageHandler = &messageHandler;

        handlers[ObjectType::HistogramValue] = &histogramValueHandler;
        handlers[ObjectType::DiscreteValue] = &discreteValueHandler;
        handlers[ObjectType::FragilityValue] = &fragilityValueHandler;
        handlers[ObjectType::Message] = &messageHandler;
        handlers[ObjectType::Evaluation] = &evaluationHandler;
        handlers[ObjectType::ProbabilityValue] = &probabilityValueHandler;
        handlers[ObjectType::StandardNormal] = &standardNormalHandler;
        handlers[ObjectType::Stochast] = &stochastHandler;
        handlers[ObjectType::Scenario] = &scenarioHandler;
        handlers[ObjectType::FragilityCurve] = &fragilityCurveHandler;
        handlers[ObjectType::ValidationReport] = &validationReportHandler;
        handlers[ObjectType::ModelParameter] = &modelParameterHandler;
        handlers[ObjectType::LimitStateFunction] = &limitStateFunctionHandler;
        handlers[ObjectType::ProbabilityLimitStateFunction] = &probabilityLimitStateFunctionHandler;
        handlers[ObjectType::CombinedLimitStateFunction] = &combinedLimitStateFunctionHandler;
        handlers[ObjectType::ReliabilityResult] = &reliabilityResultHandler;
        handlers[ObjectType::ConvergenceReport] = &convergenceReportHandler;
        handlers[ObjectType::ConditionalValue] = &conditionalValueHandler;
        handlers[ObjectType::ContributingStochast] = &contributingStochastHandler;
        handlers[ObjectType::Alpha] = &alphaHandler;
        handlers[ObjectType::StochastPoint] = &stochastPointHandler;
        handlers[ObjectType::DesignPoint] = &designPointHandler;

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
            objectType == ObjectType::FragilityCurve ||
            objectType == ObjectType::Scenario ||
            objectType == ObjectType::ValidationReport ||
            objectType == ObjectType::ModelParameter ||
            objectType == ObjectType::LimitStateFunction ||
            objectType == ObjectType::ProbabilityLimitStateFunction ||
            objectType == ObjectType::CombinedLimitStateFunction ||
            objectType == ObjectType::ReliabilityResult ||
            objectType == ObjectType::ConvergenceReport ||
            objectType == ObjectType::ContributingStochast ||
            objectType == ObjectType::ConditionalValue ||
            objectType == ObjectType::Alpha ||
            objectType == ObjectType::StochastPoint ||
            objectType == ObjectType::DesignPoint ||
            objectType == ObjectType::UncertaintyResult;
    }
}

