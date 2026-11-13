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
    using namespace Deltares::Logging;

    void AdaptiveParticleSwarmOptimizationSettings::validate(ValidationReport& report) const
    {
        ValidationSupport::checkMinimum(report, 0.0, Beta, "Beta");
        ValidationSupport::checkMaximum(report, 1.0, Beta, "Beta");
        ValidationSupport::checkMinimum(report, 0.0, CrossOver, "CrossOver");
        ValidationSupport::checkMaximum(report, 1.0, CrossOver, "CrossOver");
        ValidationSupport::checkMinimum(report, 0.1, Delta, "Delta");
        ValidationSupport::checkMaximum(report, 0.99, Delta, "Delta");
        ValidationSupport::checkMinimum(report, 0.0, DifferentialWeight, "DifferentialWeight");
        ValidationSupport::checkMaximum(report, 2.0, DifferentialWeight, "DifferentialWeight");
        generation_counters.validate(report);

        SearchArea.validate(report);
    }
}

