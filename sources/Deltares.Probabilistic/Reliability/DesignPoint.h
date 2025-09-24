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

#include <string>
#include <vector>

#include "../Model/Evaluation.h"
#include "../Logging/Message.h"
#include "../Model/StochastPoint.h"
#include "../Statistics/StandardNormal.h"
#include "ReliabilityResult.h"
#include "ConvergenceReport.h"
#include "DesignPointIds.h"

namespace Deltares::Reliability
{
    /**
     * \brief Design point, result of a reliability calculation
     */
    class DesignPoint : public Models::StochastPoint
    {
    public:
        std::string Identifier;

        /**
         * user defined identifier
         */
        std::shared_ptr<DesignPointIds> Ids = nullptr;

        std::vector<std::shared_ptr<DesignPoint>> ContributingDesignPoints;
        std::vector<std::shared_ptr<ReliabilityResult>> ReliabililityResults;
        std::vector<std::shared_ptr<Models::Evaluation>> Evaluations;
        std::vector<std::shared_ptr<Logging::Message>> Messages;

        std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

        double getFailureProbability() { return Statistics::StandardNormal::getQFromU(this->Beta); }
        double getNonFailureProbability() { return Statistics::StandardNormal::getPFromU(this->Beta); }

        void expandContributions();
        void correctFragilityCurves() const;

        /**
         * \brief Gets the total model runs including contributing design points
         * \return Total model runs
         */
        int getTotalModelRuns() const;

        /**
         * \brief Gets all stochasts in a number of design points in the design point without duplicates
         * \param designPoints design points
         * \return All stochasts without doubling
         */
        static std::vector<std::shared_ptr<Statistics::Stochast>> getUniqueStochasts(const std::vector<std::shared_ptr<Reliability::DesignPoint>>& designPoints);

        private:
            void expandFragilityCurves();
            void expandStochastRealization(std::shared_ptr<Models::StochastPointAlpha> stochastRealization);
            std::shared_ptr<Models::StochastPointAlpha> getStochastPoint(std::shared_ptr<Models::StochastPointAlpha> alphaRealization);
            void updateVariableStochasts(std::shared_ptr<StochastPoint> fragilityCurveAlpha) const;
            void updateVariableStochast(std::shared_ptr<StochastPoint> fragilityCurveAlpha,
                std::shared_ptr<Models::StochastPointAlpha> alpha) const;
    };
}

