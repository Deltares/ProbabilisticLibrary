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

#include <cmath>
#include "GumbelCopula.h"
#include "../Math/RootFinders/BisectionRootFinder.h"
#include "../Logging/ValidationSupport.h"

namespace Deltares::Statistics
{
    using namespace Deltares::Logging;

    double GumbelCopula::copulaRootFunc(double u, double v, double t) const
    {
        const double ut = -log(u);
        const double vt = -log(v);
        const double k = theta;

        double factor1 = exp( -pow( pow(ut, k) + pow(vt, k), (1.0 / k)));
        double factor2 = pow(1.0 + pow(ut / vt, k), (1.0 / k - 1.0));

        double out1 = factor1 * factor2 / v;
        out1 = out1 - t;
        return out1;
    }

    void GumbelCopula::update(const double& u, double& t) const
    {
        if (theta > 400.0)
        {
            t = u;
            return;
        }

        Numeric::RootFinderMethod method = [this, u, t](double v)
            {
                return copulaRootFunc(v, u, t);
            };

        constexpr double toleranceBisection = 0.00001;
        auto bisection = Numeric::BisectionRootFinder(toleranceBisection);

        double minStart = 1e-4;
        double maxStart = 1.0 - 1e-4;

        t = bisection.CalculateValue(minStart, maxStart, 0.0, method);
        if (t < minStart)
        {
            minStart = 1e-25;
            maxStart = 2e-4;
            t = bisection.CalculateValue(minStart, maxStart, 0.0, method);
        }
        else if (t > maxStart)
        {
            minStart = 1.0 - 2e-4;
            maxStart = 1.0 - 1e-9;
            t = bisection.CalculateValue(minStart, maxStart, 0.0, method);
        }
    }

    void GumbelCopula::validate(ValidationReport & report) const
    {
        ValidationSupport::checkMinimum(report, 1.0, theta, "Theta", "Gumbel copula", MessageType::Error);
    }
}

