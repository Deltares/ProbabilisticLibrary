// Copyright (C) Stichting Deltares 2018. All rights reserved.
//
// This file is part of the Macro Stability kernel.
//
// The Macro Stability kernel is free software: you can redistribute it and/or modify
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

#include "GeneticAlgorithmSettings.h"
#include "../Logging/ValidationSupport.h"

#include <cmath>

namespace Deltares::Optimization
{
    void GeneticAlgorithmSettings::validate(Logging::ValidationReport& report) const
    {
        using Logging::ValidationSupport;

        ValidationSupport::checkMinimumInt(report, 1, PopulationCount, "PopulationCount");
        ValidationSupport::checkMinimumInt(report, 0, EliteCount, "EliteCount");
        ValidationSupport::checkMaximumNonInclusive(report, PopulationCount, EliteCount, "EliteCount");
        ValidationSupport::checkMinimumInt(report, 1, GenerationCount, "GenerationCount");

        const auto checkFinite = [&report](double value, const std::string& name)
        {
            ValidationSupport::checkNotNaN(report, value, name);
            ValidationSupport::checkFinite(report, value, name);
        };
        const auto checkFraction = [&report, &checkFinite](double value, const std::string& name)
        {
            checkFinite(value, name);
            ValidationSupport::checkMinimum(report, 0.0, value, name);
            ValidationSupport::checkMaximum(report, 1.0, value, name);
        };
        checkFraction(MutationRate, "MutationRate");
        checkFraction(CrossOverScatterFraction, "CrossOverScatterFraction");
        checkFraction(CrossOverSinglePointFraction, "CrossOverSinglePointFraction");
        checkFraction(CrossOverDoublePointFraction, "CrossOverDoublePointFraction");
        checkFraction(MutationJumpFraction, "MutationJumpFraction");
        checkFraction(MutationCreepFraction, "MutationCreepFraction");
        checkFraction(MutationInverseFraction, "MutationInverseFraction");
        checkFinite(MutationCreepReduction, "MutationCreepReduction");
        ValidationSupport::checkMinimum(report, 0.0, MutationCreepReduction, "MutationCreepReduction");

        if (std::abs(CrossOverScatterFraction + CrossOverSinglePointFraction + CrossOverDoublePointFraction - 1.0) > 1e-12)
        {
            ValidationSupport::add(report, "Crossover fractions must sum to one.");
        }
        if (std::abs(MutationJumpFraction + MutationCreepFraction + MutationInverseFraction - 1.0) > 1e-12)
        {
            ValidationSupport::add(report, "Mutation fractions must sum to one.");
        }

        const auto checkEnum = [&report](int value, int maximum, const std::string& name)
        {
            ValidationSupport::checkMinimumInt(report, 0, value, name);
            ValidationSupport::checkMaximumInt(report, maximum, value, name);
        };
        checkEnum(static_cast<int>(InitializePopulationMethod), 1, "InitializePopulationMethod");
        checkEnum(static_cast<int>(CrossOverRandomFrequency), 2, "CrossOverRandomFrequency");
        checkEnum(static_cast<int>(MutationRandomFrequency), 2, "MutationRandomFrequency");
        checkEnum(static_cast<int>(CrossOverRandomEffect), 1, "CrossOverRandomEffect");
        checkEnum(static_cast<int>(MutationRandomEffect), 1, "MutationRandomEffect");

        ValidationSupport::checkNotEmpty(report, SearchArea.Dimensions.size(), "SearchArea");
        SearchArea.validate(report);
        for (const auto& dimension : SearchArea.Dimensions)
        {
            checkFinite(dimension.MinValue, "MinValue");
            checkFinite(dimension.MaxValue, "MaxValue");
            checkFinite(dimension.MaxValue - dimension.MinValue, "Search interval");
        }
    }
}
