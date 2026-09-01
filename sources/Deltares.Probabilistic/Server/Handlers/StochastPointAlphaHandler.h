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

#include "FragilityCurveHandler.h"
#include "StoredObjectHandler.h"
#include "../../Model/StochastPointAlpha.h"
#include "../../Reliability/FragilityCurve.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ModelInputParameter
     */
    class StochastPointAlphaHandler : public StoredObjectHandler<Models::StochastPointAlpha>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::Alpha;
        }

        double GetValue(const std::shared_ptr<Models::StochastPointAlpha>& alpha, const std::string& property_) override
        {
            if (property_ == "alpha") return alpha->Alpha;
            else if (property_ == "alpha_correlated") return alpha->AlphaCorrelated;
            else if (property_ == "u") return alpha->U;
            else if (property_ == "x") return alpha->X;
            else if (property_ == "influence_factor") return alpha->InfluenceFactor;
            else return StoredObjectHandler::GetValue(alpha, property_);
        }

        void SetValue(const std::shared_ptr<Models::StochastPointAlpha>& alpha, const std::string& property_, double value) override
        {
            if (property_ == "alpha") alpha->Alpha = value;
            else if (property_ == "u") alpha->U = value;
            else if (property_ == "x") alpha->X = value;
            else if (property_ == "alpha_correlated") alpha->AlphaCorrelated = value;
            else if (property_ == "influence_factor") alpha->InfluenceFactor = value;
            else StoredObjectHandler::SetValue(alpha, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Models::StochastPointAlpha>& alpha, const std::string& property_) override
        {
            if (property_ == "index") return alpha->Index;
            else return StoredObjectHandler::GetIntValue(alpha, property_);
        }

        void SetIntValue(const std::shared_ptr<Models::StochastPointAlpha>& alpha, const std::string& property_, int value) override
        {
            if (property_ == "variable") alpha->Stochast = stochastHandler->GetObject(value);
            else if (property_ == "index") alpha->Index = value;
            else StoredObjectHandler::SetIntValue(alpha, property_, value);
        }

        int GetIdValue(const std::shared_ptr<Models::StochastPointAlpha>& alpha, const std::string& property_) override
        {
            if (property_ == "variable")
            {
                if (alpha->Stochast == nullptr) return 0;

                std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = std::dynamic_pointer_cast<Reliability::FragilityCurve>(alpha->Stochast);
                if (fragilityCurve != nullptr)
                {
                    return fragilityCurveHandler->GetObjectId(fragilityCurve);
                }
                else
                {
                    return stochastHandler->GetObjectId(alpha->Stochast);
                }
            }
            else return StoredObjectHandler::GetIdValue(alpha, property_);
        }

        std::string GetStringValue(const std::shared_ptr<Models::StochastPointAlpha>& alpha, const std::string& property_) override
        {
            if (property_ == "identifier") return alpha->getIdentifier();
            else return StoredObjectHandler::GetStringValue(alpha, property_);
        }

        StochastHandler* stochastHandler = nullptr;
        FragilityCurveHandler* fragilityCurveHandler = nullptr;
    };
}

