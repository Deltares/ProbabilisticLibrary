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
#include <mutex>
#include <string>
#include <vector>

#include "ObjectHandler.h"
#include "ObjectHandlerStorage.h"
#include "../../Statistics/HistogramValue.h"

namespace Deltares::Server
{
    class HistogramValueHandler : public ObjectHandler
    {
    private:
        ObjectHandlerStorage storage;
    public:

        void SetAdmin(ObjectHandlerAdmin* admin) override
        {
            storage.SetAdmin(admin, ObjectType::HistogramValue);
        }

        int Create() override
        {
            return storage.Create(std::make_shared<Statistics::HistogramValue>());
        }

        void Destroy(int id) override
        {
            storage.Destroy(id);
        }

        int GetObjectId(const std::shared_ptr<Statistics::HistogramValue>& histogramValue, int newId)
        {
            return storage.GetObjectId(histogramValue, newId);
        }

        std::shared_ptr<Statistics::HistogramValue> GetObject(int id)
        {
            return storage.GetObject(id);
        }

        double GetValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Statistics::HistogramValue> histogramValue = storage.GetObject(id);

            if (property_ == "lower_bound") return histogramValue->LowerBound;
            else if (property_ == "upper_bound") return histogramValue->UpperBound;
            else if (property_ == "amount") return histogramValue->Amount;
            else return ObjectHandler::GetValue(id, property_);
        }

        void SetValue(int id, const std::string& property_, double value) override
        {
            std::shared_ptr<Statistics::HistogramValue> histogramValue = storage.GetObject(id);

            if (property_ == "lower_bound") histogramValue->LowerBound = value;
            else if (property_ == "upper_bound") histogramValue->UpperBound = value;
            else if (property_ == "amount") histogramValue->Amount = value;
            else ObjectHandler::SetValue(id, property_, value);

            histogramValue->setDirty();
        }
    };
}

