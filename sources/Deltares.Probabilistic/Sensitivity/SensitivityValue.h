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

#include "../Statistics/Stochast.h"

namespace Deltares::Sensitivity
{
    /**
     * \brief Contains the results of the sensitivity for a certain input variable
     */
    class SensitivityValue
    {
    public:
        /**
         * \brief The input variable
         */
        std::shared_ptr<Statistics::Stochast> stochast = nullptr;

        /**
         * \brief Array index of the input variable if this is an array variable
         */
        int arrayIndex = -1;

        /**
         * \brief First order Sobol index
         */
        double firstOrderIndex = std::nan("");

        /**
         * \brief Total Sobol index
         */
        double totalIndex = std::nan("");

        /**
         * \brief Value of the output variable if the input variable gets a low value
         * \remarks A sensitivity value is always part of a SensitivityResult, which refers to an output variable
         */
        double low = std::nan("");

        /**
         * \brief Value of the output variable if the input variable gets a medium value
         * \remarks A sensitivity value is always part of a SensitivityResult, which refers to an output variable
         */
        double medium = std::nan("");

        /**
         * \brief Value of the output variable if the input variable gets a medium value
         * \remarks A sensitivity value is always part of a SensitivityResult, which refers to an output variable
         */
        double high = std::nan("");

        /**
         * \brief Gets the identification string of the input variable
         */
        std::string getIdentifier() const
        {
            if (this->stochast == nullptr)
            {
                return "";
            }
            else
            {
                return this->stochast->getIndexedStochastName(this->arrayIndex);
            }
        }
    };
}

