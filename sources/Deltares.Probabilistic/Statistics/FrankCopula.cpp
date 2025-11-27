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

#include "FrankCopula.h"

#include "../Math/RootFinders/BisectionRootFinder.h"

namespace Deltares::Statistics
{
    double FrankCopula::copulaRootFunc(double u, double v, double t) const
    {
        const double C = -1.0 / alpha;
        const double gx = 1.0 + expm1(-alpha*u) * expm1(-alpha*v) / expm1(-alpha);
        const double gxdx = -expm1(-alpha * u) * exp(-alpha * v) * alpha / expm1(-alpha);

        double out1 = gxdx * C / gx;
        out1 = out1 - t;
        return out1;
    }

    /// summary
    /// helper function for copulaRootFunc exp(x) - 1.0
    /// @param x 
    /// @return exp(x) - 1.0
    double FrankCopula::expm1(const double x)
    {
        return exp(x) - 1.0;
    }

    void FrankCopula::update(const double& u, double& t) const
    {
        Numeric::RootFinderMethod method = [this, u, t](double v)
            {
                return copulaRootFunc(v, u, t);
            };

        constexpr double toleranceBisection = 0.00001;
        auto bisection = Numeric::BisectionRootFinder(toleranceBisection);

        double minStart = 0.0;
        double maxStart = 1.0;

        t = bisection.CalculateValue(minStart, maxStart, 0.0, method);
    }
}


