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

namespace Deltares::Statistics
{
    double GumbelCopula::copulaC(double u, double v, double t) const
    {
        const double ut = -log(u);
        const double vt = -log(v);
        const double k = alpha;

        double out1 = (pow(v, (-1))*pow(exp(-(pow(ut , k) + pow( vt, k))), (1 / k)))*pow(1 + pow(ut / vt, k), (1 / k - 1));
        out1 = out1 - t;
        return out1;
    }

    void GumbelCopula::update(const double& u, double& t) const
    {
        Numeric::RootFinderMethod method = [this, u, t](double v)
            {
                return copulaC(u, t, v);
            };

        constexpr double toleranceBisection = 0.00001;
        auto bisection = Numeric::BisectionRootFinder(toleranceBisection);

        double minStart = 1e-4;
        double maxStart = 1-1e-4;

        t = bisection.CalculateValue(minStart, maxStart, 0.0, method);

    }
}

