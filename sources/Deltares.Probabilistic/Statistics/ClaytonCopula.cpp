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

#include "ClaytonCopula.h"

#include <cmath>
#include <format>

namespace Deltares::Statistics
{
    void ClaytonCopula ::update(const double& u, double& t) const
    {
        if (theta == -1.0)
        {
            t = 1.0 - u;
            return;
        }

        if (theta < 1.0e8)
        {
            t = u * pow(pow(t, -theta / (1.0 + theta)) - 1.0 + pow(u, theta), -1.0 / theta);
        }
        else
        {
            t = u * (1.0 - (log1p(-u) + log(u)) / theta);
        }
    }

    void ClaytonCopula::validate(Logging::ValidationReport& report) const
    {
        if (theta == 0.0 || theta < -1.0)
        {
            auto msg = std::make_shared<Logging::Message>();
            msg->Text = std::format("Theta in Clayton copula should be >= -1.0 and <> 0.0, but is {:.6F}", theta);
            msg->Type = Logging::MessageType::Error;
            msg->Subject = "Clayton copula";
            report.messages.push_back(msg);
        }
    }

}

