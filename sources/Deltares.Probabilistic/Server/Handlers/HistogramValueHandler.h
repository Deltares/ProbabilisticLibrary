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
#include "../../Statistics/HistogramValue.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class HistogramValue
     */
    class HistogramValueHandler : public StoredObjectHandler<Statistics::HistogramValue>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::HistogramValue;
        }

        double GetValue(const std::shared_ptr<Statistics::HistogramValue>& histogramValue, const std::string& property_) override
        {
            if (property_ == "lower_bound") return histogramValue->LowerBound;
            else if (property_ == "upper_bound") return histogramValue->UpperBound;
            else if (property_ == "amount") return histogramValue->Amount;
            else return StoredObjectHandler::GetValue(histogramValue, property_);
        }

        void SetValue(const std::shared_ptr<Statistics::HistogramValue>& histogramValue, const std::string& property_, double value) override
        {
            if (property_ == "lower_bound") histogramValue->LowerBound = value;
            else if (property_ == "upper_bound") histogramValue->UpperBound = value;
            else if (property_ == "amount") histogramValue->Amount = value;
            else StoredObjectHandler::SetValue(histogramValue, property_, value);

            histogramValue->setDirty();
        }
    };
}

