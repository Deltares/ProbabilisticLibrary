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

#include "AdaptiveParticleSwarmOptimizationSettings.h"

namespace Deltares::Optimization
{
    void AdaptiveParticleSwarmOptimizationSettings::validate(Logging::ValidationReport& report) const
    {
        Logging::ValidationSupport::checkMinimum(report, 0.0, Beta, "Beta");
        Logging::ValidationSupport::checkMaximum(report, 1.0, Beta, "Beta");
        Logging::ValidationSupport::checkMinimum(report, 0.0, CrossOver, "CrossOver");
        Logging::ValidationSupport::checkMaximum(report, 1.0, CrossOver, "CrossOver");
        Logging::ValidationSupport::checkMinimum(report, 0.0, Delta, "Delta");
        Logging::ValidationSupport::checkMaximum(report, 1.0, Delta, "Delta");
        Logging::ValidationSupport::checkMinimum(report, 0.0, DifferentialWeight, "DifferentialWeight");
        Logging::ValidationSupport::checkMaximum(report, 1.0, DifferentialWeight, "DifferentialWeight");
        Logging::ValidationSupport::checkMinimumInt(report, 1, GenerationCount, "GenerationCount");
        Logging::ValidationSupport::checkMinimumInt(report, 1, EliteCount, "EliteCount");
        Logging::ValidationSupport::checkMinimumInt(report, 1, PopulationCount, "PopulationCount");
        Logging::ValidationSupport::checkMinimumInt(report, 1, StopAfterNonImprovingGenerations, "StopAfterNonImprovingGenerations");
    }
}

