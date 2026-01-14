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

#include "GaussianCopula.h"
#include "StandardNormal.h"
#include "../Logging/ValidationSupport.h"
#include <cmath>

namespace Deltares::Statistics
{
    using namespace Deltares::Logging;

    void GaussianCopula::update_uspace(const double& a, double& b) const
    {
        b = a * rho + b * sqrt(1.0 - rho * rho);
    }

    void GaussianCopula::update(const double& u, double& t) const
    {
        double a = StandardNormal::getUFromP(u);
        double b = StandardNormal::getUFromP(t);
        update_uspace(a, b);
        t = StandardNormal::getUFromP(b);
    }

    void GaussianCopula::validate(ValidationReport& report) const
    {
        ValidationSupport::checkMinimum(report, -1.0, rho, "Rho", "Gaussian copula", MessageType::Error);
        ValidationSupport::checkMaximum(report,  1.0, rho, "Rho", "Gaussian copula", MessageType::Error);
    }

}

