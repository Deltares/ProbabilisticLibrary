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

#include "Scenario.h"

namespace Deltares::Statistics
{
    std::vector<std::shared_ptr<Logging::Message>> Scenario::validate() const
    {
        const double margin = 1E-10;

        std::vector<std::shared_ptr<Logging::Message>> messages;

        if (std::isnan(this->probability))
        {
            messages.push_back(std::make_shared<Logging::Message>(Logging::MessageType::Error, "Scenario probability should not be nan."));
        }
        else if (this->probability < -margin || this->probability > 1 + margin)
        {
            messages.push_back(std::make_shared<Logging::Message>(Logging::MessageType::Error, "Scenario probability should be in range [0, 1]."));
        }

        return messages;
    }
}

