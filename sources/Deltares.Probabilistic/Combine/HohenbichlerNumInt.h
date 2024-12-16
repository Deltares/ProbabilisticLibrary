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
#pragma once
#include <utility>
#include <vector>
#include "CombineType.h"
#include "../Reliability/DesignPoint.h"
#include "../Statistics/SelfCorrelationMatrix.h"

namespace Deltares {
    namespace Reliability {

        class HohenbichlerNumInt
        {
        public:
            std::shared_ptr<DesignPoint> AlphaHohenbichler(const std::shared_ptr<DesignPoint>& designPoint1, const std::shared_ptr<DesignPoint>& designPoint2,
                const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts,
                const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelation, const combineAndOr system);
        private:
            static double BetaHohenbichler(double dp1, double dp2, const double rho, const combineAndOr system);
            static double Hohenbichler(const double dp1, const double dp2, const double rho, const combineAndOr system);
            static DesignPoint GetRealization(const double beta, const std::vector<std::shared_ptr<Models::StochastPointAlpha>>& alpha);
            static DesignPoint GetRealization(const double beta, const std::vector<std::shared_ptr<Models::StochastPointAlpha>>& alpha, const std::vector<double>& values);
        };
    }
}
