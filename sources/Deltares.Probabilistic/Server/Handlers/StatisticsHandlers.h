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
#pragma once

#include "../ProjectEntries.h"

#include "ContributingStochastHandler.h"
#include "CopulaCorrelationHandler.h"
#include "CorrelationMatrixHandler.h"
#include "DiscreteValueHandler.h"
#include "FragilityValueHandler.h"
#include "HistogramValueHandler.h"
#include "ModelHandlers.h"
#include "ProbabilityValueHandler.h"
#include "ScenarioHandler.h"
#include "SelfCorrelationMatrixHandler.h"
#include "StandardNormalHandler.h"
#include "StochastHandler.h"
#include "ReliabilityProjectHandler.h"
#include "StochastPointHandler.h"
#include "StochastSettingsHandler.h"

namespace Deltares::Server
{
    class StatisticsHandlers
    {
    public:
        void InitializeHandlers(std::unordered_map<ObjectType, ObjectHandler*>& handlers, ModelHandlers* modelHandlers);

        HistogramValueHandler histogramValueHandler;
        DiscreteValueHandler discreteValueHandler;
        FragilityValueHandler fragilityValueHandler;
        ProbabilityValueHandler probabilityValueHandler;
        StandardNormalHandler standardNormalHandler;
        StochastHandler stochastHandler;
        ScenarioHandler scenarioHandler;
        ContributingStochastHandler contributingStochastHandler;
        ConditionalValueHandler conditionalValueHandler;
        CorrelationMatrixHandler correlationMatrixHandler;
        CopulaCorrelationHandler copulaCorrelationHandler;
        SelfCorrelationMatrixHandler selfCorrelationMatrixHandler;
        StochastSettingsHandler stochastSettingsHandler;
        StochastPointAlphaHandler alphaHandler;
        StochastPointHandler stochastPointHandler;
        ModelProjectSettingsHandler modelProjectSettingsHandler;
        ModelProjectHandler modelProjectHandler;
        RunProjectSettingsHandler runProjectSettingsHandler;
        RunProjectHandler runProjectHandler;
    };
}
