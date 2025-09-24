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
#pragma once
#include <string>
#include <memory>

#include "Stochast.h"
#include "../Logging/Message.h"

namespace Deltares::Statistics
{
    /**
     * \brief Defines the contribution of a design point, when it is combined with other design point points in an exclusive way
     * \remarks The class ExcludingCombineProject connects design points and scenarios
     */
    class Scenario
    {
    public:
        /**
         * \brief Name of the scenario 
         */
        std::string name = "";

        /**
         * \brief Contribution of a design point in ExcludingCombineProject, defined as fraction between 0 and 1
         */
        double probability = 0.0;

        /**
         * \brief Parameter from which the probability originates, usually a stochastic variable with a discrete distribution
         * \remarks When this parameter is set, it will appear in the alpha values of a combined design point
         */
        std::shared_ptr<Stochast> parameter = nullptr;

        /**
         * \brief X-Value of a discrete value if the parameter in this scenario refers to a stochastic variable with discrete distribution
         */
        double parameterValue = std::nan("");

        /**
         * \brief Validates this scenario
         * @return validation messages
         */
        std::vector<std::shared_ptr<Logging::Message>> validate() const;

    };
}

