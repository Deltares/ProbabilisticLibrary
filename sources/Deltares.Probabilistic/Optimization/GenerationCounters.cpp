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

#include "GenerationCounters.h"

namespace Deltares::Optimization
{
    GenerationCounters::GenerationCounters(int generation_count, int elite_count, int population_count, int stop_after_non_improving_generations) :
        generation_count(generation_count), elite_count(elite_count), population_count(population_count),
        stop_after_non_improving_generations(stop_after_non_improving_generations) {}

    void GenerationCounters::validate(Logging::ValidationReport& report) const
    {
        using Logging::ValidationSupport;

        ValidationSupport::checkMinimumInt(report, 1, population_count, "population_count");
        ValidationSupport::checkMinimumInt(report, 0, elite_count, "elite_count");
        ValidationSupport::checkMaximumNonInclusive(report, population_count, elite_count, "elite_count");
        ValidationSupport::checkMinimumInt(report, 1, generation_count, "generation_count");
    }
}
