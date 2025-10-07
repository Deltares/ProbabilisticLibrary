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
#include "DirectionReliabilitySettings.h"

namespace Deltares::Reliability
{
    /**
     * \brief Reports whether the settings have valid values
     * \param report Report in which the validity is reported
     */
    void DirectionReliabilitySettings::validate(Logging::ValidationReport& report) const
    {
        Logging::ValidationSupport::checkMinimum(report, 0.01, Dsdu, "dsdu");
        Logging::ValidationSupport::checkMinimum(report, 0.00001, EpsilonUStepSize, "epsilon u step size");
        Logging::ValidationSupport::checkMinimum(report, 0.00001, EpsilonZStepSize, "epsilon z step size");
        Logging::ValidationSupport::checkMinimumInt(report, 1, MaximumIterations, "maximum iterations");
        Logging::ValidationSupport::checkMinimum(report, 1, MaximumLengthU, "maximum length u");
    }


    int DirectionReliabilitySettings::SectionCount() const
    {
        int sectionsCount = static_cast<int>(MaximumLengthU / Dsdu);
        if (sectionsCount * Dsdu < MaximumLengthU) { sectionsCount++; }
        return sectionsCount;
    }

}


