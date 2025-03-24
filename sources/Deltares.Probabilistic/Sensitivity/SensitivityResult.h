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
#include "../Model/Evaluation.h"
#include "../Model/Message.h"

namespace Deltares::Sensitivity
{
    /**
     * \brief Contains the results of a sensitivity calculation
     */
    class SensitivityResult
    {
    public:
        /**
         * \brief The stochast describing the variance of the sensitivity 
         */
        std::shared_ptr<Statistics::Stochast> stochast = nullptr;

        /**
         * \brief Gets the identifier of the result
         */
        std::string getIdentifier() const
        {
            return stochast != nullptr ? stochast->name : "";
        }

        /**
         * \brief List of evaluations corresponding with the requested quantiles
         */
        std::vector<std::shared_ptr<Models::Evaluation>> quantileEvaluations;

        /**
         * \brief List of evaluations calculated during sensitivity analysis
         */
        std::vector<std::shared_ptr<Models::Evaluation>> evaluations;

        /**
         * \brief List of messages raised during sensitivity analysis
         */
        std::vector<std::shared_ptr<Models::Message>> messages;
    };
}

