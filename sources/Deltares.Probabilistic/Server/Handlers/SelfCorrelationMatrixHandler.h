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

#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Statistics/Stochast.h"
#include "../../Statistics/SelfCorrelationMatrix.h"
#include "StochastHandler.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class SelfCorrelationMatrix
     */
    class SelfCorrelationMatrixHandler : public StoredObjectHandler<Statistics::SelfCorrelationMatrix>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::SelfCorrelationMatrix;
        }

        double GetIntArgValue(const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix, int argument, const std::string& property_) override
        {
            if (property_ == "rho") return selfCorrelationMatrix->getSelfCorrelation(stochastHandler->GetObject(argument));
            else return StoredObjectHandler::GetIntArgValue(selfCorrelationMatrix, argument, property_);
        }

        void SetIntArgValue(const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix, int argument, const std::string& property_, double value) override
        {
            if (property_ == "rho") selfCorrelationMatrix->setSelfCorrelation(stochastHandler->GetObject(argument), value);
            else StoredObjectHandler::SetIntArgValue(selfCorrelationMatrix, argument, property_, value);
        }

        StochastHandler* stochastHandler = nullptr;
    };
}

