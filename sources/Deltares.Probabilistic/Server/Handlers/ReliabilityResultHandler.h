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
#include "../../Reliability/ReliabilityResult.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ReliabilityResult
     */
    class ReliabilityResultHandler : public StoredObjectHandler<Reliability::ReliabilityResult>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::ReliabilityResult;
        }

        double GetValue(const std::shared_ptr<Reliability::ReliabilityResult>& result, const std::string& property_) override
        {
            if (property_ == "reliability_index") return result->Reliability;
            else if (property_ == "convergence") return std::isnan(result->ConvBeta) ? result->Variation : result->ConvBeta;
            else if (property_ == "variation") return result->Variation;
            else if (property_ == "contribution") return result->Contribution;
            else return StoredObjectHandler::GetValue(result, property_);
        }

        void SetValue(const std::shared_ptr<Reliability::ReliabilityResult>& result, const std::string& property_, double value) override
        {
            if (property_ == "reliability_index") result->Reliability = value;
            else if (property_ == "convergence") result->ConvBeta = value;
            else if (property_ == "variation") result->Variation = value;
            else if (property_ == "contribution") result->Contribution = value;
            else StoredObjectHandler::SetValue(result, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Reliability::ReliabilityResult>& result, const std::string& property_) override
        {
            if (property_ == "index") return result->Index;
            else if (property_ == "samples") return static_cast<int>(result->Samples);
            else return StoredObjectHandler::GetIntValue(result, property_);
        }

        void SetIntValue(const std::shared_ptr<Reliability::ReliabilityResult>& result, const std::string& property_, int value) override
        {
            if (property_ == "index") result->Index = value;
            else if (property_ == "samples") result->Samples = value;
            else StoredObjectHandler::SetValue(result, property_, value);
        }
    };
}

