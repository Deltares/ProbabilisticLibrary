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

#include "DiagonalBandCopula.h"

namespace Deltares::Statistics
{
    void DiagonalBandCopula::update(const double& u, double& t) const
    {
        double Finv;
        if (u < 1.0 - alpha and t < 1.0 - u / (1.0 - alpha))
        {
            Finv = (1.0 - alpha) * t;

        }
        else if (u > alpha and t > (1.0 - u) / (1.0 - alpha))
        {
            Finv = (1.0 - alpha) * t + alpha;
        }
        else
        {
            Finv = 2.0 * (1.0 - alpha) * t + u - 1.0 + alpha;
        }
        t = Finv;
    }

    void DiagonalBandCopula::validate(Logging::ValidationReport& report) const
    {
        if (alpha < 0.0 || alpha > 1.0)
        {
            auto msg = std::make_shared<Logging::Message>();
            msg->Text = "Alpha in Diagonal Band copula should be in [0.0, 1.0], but is " + std::to_string(alpha);
            msg->Type = Logging::MessageType::Error;
            report.messages.push_back(msg);
        }
    }

}

