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
#include <vector>

#include "DistributionType.h"
#include "VariableStochastValue.h"
#include "Distributions/Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class VariableStochastValuesSet
        {
        public:
            std::vector<std::shared_ptr<VariableStochastValue>> StochastValues;
            void initializeForRun();
            std::shared_ptr<StochastProperties> getInterpolatedStochast(double x);
            bool isVarying(DistributionType distributionType);
            void copyFrom(std::shared_ptr<VariableStochastValuesSet> source);
        private:
            std::vector<double> xValues;
            std::vector<double> locations;
            std::vector<double> scales;
            std::vector<double> minimums;
            std::vector<double> maximums;
            std::vector<double> shapes;
            std::vector<double> shapesB;
            std::vector<double> shifts;
            std::vector<double> shiftsB;
            std::vector<double> observations;
        };
    }
}

