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

#include "ThreeFractions.h"
#include "../Utils/ProbabilisticLibraryException.h"
#include <cmath>

namespace Deltares::Optimization
{
    using namespace Deltares::Logging;

    ThreeFractions::ThreeFractions(double a, double b, double c) : a(a), b(b), c(c) {}

    ThreeFractions::ThreeFractions(double a, double b, double c, std::array<std::string,3> names)
    : a(a), b(b), c(c), names(std::move(names)) {}

    void ThreeFractions::validate(ValidationReport& report) const
    {
        ValidationSupport::checkMinimum(report, 0.0, a, names[0]);
        ValidationSupport::checkMinimum(report, 0.0, b, names[1]);
        ValidationSupport::checkMinimum(report, 0.0, c, names[2]);
        ValidationSupport::checkNotNaN(report, a, names[0]);
        ValidationSupport::checkNotNaN(report, b, names[1]);
        ValidationSupport::checkNotNaN(report, c, names[2]);
        ValidationSupport::checkFinite(report, a, names[0]);
        ValidationSupport::checkFinite(report, b, names[1]);
        ValidationSupport::checkFinite(report, c, names[2]);
        if (std::abs(a + b + c - 1.0) > 1e-12)
        {
            ValidationSupport::add(report, "Fractions must sum to one.");
        }
    }

    double ThreeFractions::getValueByIndex(int index) const
    {
        double return_value;
        switch (index)
        {
        case 0: return_value = a;  break;
        case 1: return_value = b; break;
        case 2: return_value = c; break;
        default: throw Reliability::ProbabilisticLibraryException("Index out of range in ThreeFractions: ", index);
        }
        return return_value;
    }

    std::array<double, 3> ThreeFractions::getMaskRange() const
    {
        return { a, a + b, 1.0 };
    }

}

