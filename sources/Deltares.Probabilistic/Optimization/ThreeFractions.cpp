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

    void ThreeFractions::validate(ValidationReport& report) const
    {
        ValidationSupport::checkMinimum(report, 0.0, a, "a");
        ValidationSupport::checkMinimum(report, 0.0, b, "b");
        ValidationSupport::checkMinimum(report, 0.0, c, "c");
        ValidationSupport::checkNotNaN(report, a, "a");
        ValidationSupport::checkNotNaN(report, b, "b");
        ValidationSupport::checkNotNaN(report, c, "c");
        ValidationSupport::checkFinite(report, a, "a");
        ValidationSupport::checkFinite(report, b, "b");
        ValidationSupport::checkFinite(report, c, "c");
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

