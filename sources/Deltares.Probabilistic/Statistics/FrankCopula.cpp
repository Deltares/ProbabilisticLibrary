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
    void FrankCopula::update(const double& u, double& t) const
    {
        if (theta < 700.0)
        {
            const double factor = (t - 1.0) * std::exp(-theta * u);
            t = 1.0 + std::log((factor - t) / (factor * std::exp(theta) - t)) / theta;
        }
        else
        {
            t = u;
        }
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


