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

#include "StochastPointAlphaHandler.h"
#include "StoredObjectHandler.h"
#include "../../Model/StochastPoint.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class DesignPoint
     */
    class StochastPointHandler : public StoredObjectHandler<Models::StochastPoint>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::StochastPoint;
        }

        double GetValue(const std::shared_ptr<Models::StochastPoint>& stochastPoint, const std::string& property_) override
        {
            if (property_ == "beta") return stochastPoint->Beta;
            else if (property_ == "reliability_index") return stochastPoint->Beta;
            else return StoredObjectHandler::GetValue(stochastPoint, property_);
        }

        void SetValue(const std::shared_ptr<Models::StochastPoint>& stochastPoint, const std::string& property_, double value) override
        {
            if (property_ == "beta") stochastPoint->Beta = value;
            else if (property_ == "reliability_index") stochastPoint->Beta = value;
            else StoredObjectHandler::SetValue(stochastPoint, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Models::StochastPoint>& stochastPoint, const std::string& property_) override
        {
            if (property_ == "alphas_count") return static_cast<int>(stochastPoint->Alphas.size());
            else return StoredObjectHandler::GetIntValue(stochastPoint, property_);
        }

        void SetIntValue(const std::shared_ptr<Models::StochastPoint>& stochastPoint, const std::string& property_, int value) override
        {
            if (property_ == "add_alpha") stochastPoint->Alphas.push_back(alphaHandler->GetObject(value));
            else StoredObjectHandler::SetIntValue(stochastPoint, property_, value);
        }

        void SetArrayIntValue(const std::shared_ptr<Models::StochastPoint>& stochastPoint, const std::string& property_, int* values, int size) override
        {
            if (property_ == "alphas")
            {
                stochastPoint->Alphas.clear();
                for (int i = 0; i < size; i++)
                {
                    stochastPoint->Alphas.push_back(alphaHandler->GetObject(values[i]));
                }
            }
            else StoredObjectHandler::SetArrayIntValue(stochastPoint, property_, values, size);
        }

        int GetIndexedIdValue(const std::shared_ptr<Models::StochastPoint>& stochastPoint, const std::string& property_, int index) override
        {
            if (property_ == "alphas") return alphaHandler->GetObjectId(stochastPoint->Alphas[index]);
            else return StoredObjectHandler::GetIndexedIdValue(stochastPoint, property_, index);
        }

        StochastPointAlphaHandler* alphaHandler = nullptr;
    };
}

