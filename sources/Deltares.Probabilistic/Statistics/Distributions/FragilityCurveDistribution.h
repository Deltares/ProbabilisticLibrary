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

#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class FragilityCurveDistribution : public Distribution
        {
            double getMean(std::shared_ptr<StochastProperties> stochast) override;
            double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override { return true; }
            bool maintainMeanAndDeviation(std::shared_ptr<StochastProperties> stochast) override { return false; }
            void initializeForRun(std::shared_ptr<StochastProperties> stochast) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool canFit() override { return false; }
            bool isValid(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
        private:
            const double precision = 0.00001;

            bool isHorizontal(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues);
            bool startsHorizontal(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues);
            bool isAscending(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues);
            bool isDescending(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues);
            bool endsHorizontal(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues);
        };
    }
}

