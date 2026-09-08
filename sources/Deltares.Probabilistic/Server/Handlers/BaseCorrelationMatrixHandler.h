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
#include "../../Statistics/BaseCorrelation.h"
#include "StochastHandler.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class CorrelationMatrix
     */
    class BaseCorrelationMatrixHandler : public StoredObjectHandler<Statistics::BaseCorrelation>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::CorrelationMatrix;
        }

        int GetIntValue(const std::shared_ptr<Statistics::BaseCorrelation>& correlationMatrix, const std::string& property_) override
        {
            if (property_ == "count_correlations") return correlationMatrix->CountCorrelations();
            else if (property_ == "variables_count") return correlationMatrix->GetDimension();
            else return StoredObjectHandler::GetIntValue(correlationMatrix, property_);
        }

        bool GetBoolValue(const std::shared_ptr<Statistics::BaseCorrelation>& correlationMatrix, const std::string& property_) override
        {
            if (property_ == "is_identity") return correlationMatrix->IsIdentity();
            else if (property_ == "is_valid") return correlationMatrix->IsValid();
            else return StoredObjectHandler::GetBoolValue(correlationMatrix, property_);
        }

        int GetIndexedIdValue(const std::shared_ptr<Statistics::BaseCorrelation>& correlationMatrix, const std::string& property_, int index) override
        {
            if (property_ == "variables") return stochastHandler->GetObjectId(correlationMatrix->GetStochast(index));
            else return StoredObjectHandler::GetIndexedIdValue(correlationMatrix, property_, index);
        }

        void SetArrayIntValue(const std::shared_ptr<Statistics::BaseCorrelation>& correlationMatrix, const std::string& property_, int* values, int size) override
        {
            if (property_ == "variables")
            {
                std::vector<std::shared_ptr<Statistics::Stochast>> correlationMatrixStochasts;
                stochastHandler->SetIdValues(correlationMatrixStochasts, values, size);
                correlationMatrix->Init(correlationMatrixStochasts);
            }
        }

        double GetIndexedIndexedValue(const std::shared_ptr<Statistics::BaseCorrelation>& correlationMatrix, const std::string& property_, int index1, int index2) override
        {
            if (property_ == "correlation") return correlationMatrix->GetCorrelation(stochastHandler->GetObject(index1), stochastHandler->GetObject(index2)).value;
            else if (property_ == "correlation_index") return correlationMatrix->GetCorrelation(index1, index2).value;
            else return StoredObjectHandler::GetIndexedIndexedValue(correlationMatrix, property_, index1, index2);
        }

        void SetIndexedIndexedValue(const std::shared_ptr<Statistics::BaseCorrelation>& correlationMatrix, const std::string& property_, int index1, int index2, double value) override
        {
            if (property_ == "correlation") correlationMatrix->SetCorrelation(stochastHandler->GetObject(index1), stochastHandler->GetObject(index2), value, CorrelationType::Gaussian);
            else if (property_ == "correlation_index") correlationMatrix->SetCorrelation(index1, index2, value, CorrelationType::Gaussian);
            else StoredObjectHandler::SetIndexedIndexedValue(correlationMatrix, property_, index1, index2, value);
        }

        StochastHandler* stochastHandler = nullptr;
    };
}

