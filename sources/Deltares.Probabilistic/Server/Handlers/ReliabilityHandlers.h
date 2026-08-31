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

#include <unordered_map>

#include "ObjectHandler.h"
#include "ConvergenceReportHandler.h"
#include "FragilityCurveHandler.h"
#include "LimitStateFunctionHandler.h"
#include "CombinedLimitStateFunctionHandler.h"
#include "CombineProjectHandler.h"
#include "ExcludingCombineProjectHandler.h"
#include "FragilityCurveProjectHandler.h"
#include "FragilityCurveSettingsHandler.h"
#include "LengthEffectProjectHandler.h"
#include "ProbabilityLimitStateFunctionHandler.h"
#include "ReliabilityResultHandler.h"
#include "ReliabilitySettingsHandler.h"
#include "StatisticsHandlers.h"

#include "../ProjectEntries.h"

namespace Deltares::Server
{
    class ReliabilityHandlers
    {
    public:
        void InitializeHandlers(std::unordered_map<ObjectType, ObjectHandler*>& handlers, ModelHandlers* modelHandlers, StatisticsHandlers* statisticsHandlers);

        FragilityCurveHandler fragilityCurveHandler;
        DesignPointHandler designPointHandler;
        LimitStateFunctionHandler limitStateFunctionHandler;
        ProbabilityLimitStateFunctionHandler probabilityLimitStateFunctionHandler;
        CombinedLimitStateFunctionHandler combinedLimitStateFunctionHandler;
        ReliabilityResultHandler reliabilityResultHandler;
        ConvergenceReportHandler convergenceReportHandler;
        ReliabilitySettingsHandler reliabilitySettingsHandler;
        FragilityCurveSettingsHandler fragilityCurveSettingsHandler;
        CombineSettingsHandler combineSettingsHandler;
        ExcludingCombineSettingsHandler excludingCombineSettingsHandler;
        CombineProjectHandler combineProjectHandler;
        ExcludingCombineProjectHandler excludingCombineProjectHandler;
        LengthEffectProjectHandler lengthEffectProjectHandler;
        ReliabilityProjectHandler reliabilityProjectHandler;
        FragilityCurveProjectHandler fragilityCurveProjectHandler;
    private:
        std::shared_ptr<Reliability::LimitStateFunction> GetLimitStateFunction(int id);
    };
}
