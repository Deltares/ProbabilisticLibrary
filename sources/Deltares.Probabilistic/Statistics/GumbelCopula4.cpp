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

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include "GumbelCopula4.h"
#include "../Math/RootFinders/BisectionRootFinder.h"
#include "../Logging/ValidationSupport.h"

namespace Deltares::Statistics
{

    GumbelCopula4::GumbelCopula4(const double theta) : theta(theta)
    {
        random.initialize(true, 3067);
    }

    void GumbelCopula4::update(double& u, double& t)
    {
        u = (u - 0.5) * M_PI;
        const double u2 = u + M_PI_2;
        const double e = -std::log(t);
        const double inverseTheta = 1.0 / theta;
        t = std::cos(u - u2 * inverseTheta) / e;
        const double gamma = std::pow(std::sin(u2 * inverseTheta) / t, inverseTheta) * t / std::cos(u);
        u = std::exp(-std::pow(-std::log(random.next()), inverseTheta) / gamma);
        t = std::exp(-std::pow(-std::log(random.next()), inverseTheta) / gamma);
    }

    void GumbelCopula4::validate(Logging::ValidationReport & report) const
    {
        Logging::ValidationSupport::checkMinimum(report, 1.0, theta, "Theta", "Gumbel copula", Logging::MessageType::Error);
    }
}

