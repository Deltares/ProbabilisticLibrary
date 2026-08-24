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

#include "ObjectHandler.h"
#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Statistics/StandardNormal.h"
#include "../../Statistics/ProbabilityValue.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class StandardNormal
     * \remarks Another class type is used for instantiation
     */
    class StandardNormalHandler : public StoredObjectHandler<Statistics::ProbabilityValue>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::StandardNormal;
        }

        double GetValue(int id, const std::string& property_) override
        {
            if (property_ == "u_max") return Statistics::StandardNormal::UMax;
            else if (property_ == "beta_max") return Statistics::StandardNormal::BetaMax;
            else return ObjectHandler::GetValue(id, property_);
        }

        double GetArgValue(int id, const std::string& property_, double argument) override
        {
            if (property_ == "u_from_q") return Statistics::StandardNormal::getUFromQ(argument);
            else if (property_ == "u_from_p") return Statistics::StandardNormal::getUFromP(argument);
            else if (property_ == "q_from_u") return Statistics::StandardNormal::getQFromU(argument);
            else if (property_ == "p_from_u") return Statistics::StandardNormal::getPFromU(argument);
            else if (property_ == "t_from_p") return Statistics::StandardNormal::getTFromP(argument);
            else if (property_ == "p_from_t") return Statistics::StandardNormal::getPFromT(argument);
            else if (property_ == "t_from_u") return Statistics::StandardNormal::getTFromU(argument);
            else if (property_ == "u_from_t") return Statistics::StandardNormal::getUFromT(argument);
            else return ObjectHandler::GetArgValue(id, property_, argument);
        }
    };
}

