// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
#pragma once

#include <string>
#include <vector>

#include "../Model/Evaluation.h"
#include "../Model/Message.h"
#include "../Model/StochastPoint.h"
#include "../Statistics/StandardNormal.h"
#include "ReliabilityResult.h"
#include "ConvergenceReport.h"
#include "FragilityCurve.h"

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Models;

        /**
         * \brief Design point, result of a reliability calculation
         */
        class DesignPoint : public StochastPoint
        {
        public:
            std::string Identifier = "";

            std::vector<std::shared_ptr<DesignPoint>> ContributingDesignPoints;
            std::vector<std::shared_ptr<ReliabilityResult>> ReliabililityResults;
            std::vector<std::shared_ptr<Evaluation>> Evaluations;
            std::vector<std::shared_ptr<Deltares::Models::Message>> Messages;

            std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

            double getFailureProbability() { return Statistics::StandardNormal::getQFromU(this->Beta); }
            double getNonFailureProbability() { return Statistics::StandardNormal::getPFromU(this->Beta); }

            void expandContributions();
            void correctFragilityCurves();
        private:
            void expandFragilityCurves();
            void expandStochastRealization(std::shared_ptr<StochastPointAlpha> stochastRealization);
            std::shared_ptr<StochastPointAlpha> getStochastPoint(std::shared_ptr<StochastPointAlpha> alphaRealization);
            void updateVariableStochasts(std::shared_ptr<StochastPoint> fragilityCurveAlpha);
        };
    }
}

