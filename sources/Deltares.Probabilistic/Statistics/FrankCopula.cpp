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
#include <cmath>

namespace Deltares::Statistics
{
    double FrankCopula::copulaRootFunc(double u, double v, double t) const
    {
        const double C = -1.0 / theta;
        const double gx = 1.0 + expm1(-theta*u) * expm1(-theta*v) / expm1(-theta);
        const double gxdx = -expm1(-theta * u) * exp(-theta * v) * theta / expm1(-theta);

        double out1 = gxdx * C / gx;
        out1 = out1 - t;
        return out1;
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

    void FrankCopula::validate(Logging::ValidationReport& report) const
    {
        if (theta == 0.0)
        {
            auto msg = std::make_shared<Logging::Message>();
            msg->Text = "Theta in Frank copula should be <> 0.0, but is 0.0";
            msg->Type = Logging::MessageType::Error;
            report.messages.push_back(msg);
        }
    }

}


