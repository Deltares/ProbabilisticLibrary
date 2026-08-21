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
#include <vector>

#include "../../Statistics/HistogramValue.h"

namespace Deltares::Server
{
    class ObjectHandlerStorage
    {
    private:
        std::unordered_map<int, std::shared_ptr<Statistics::HistogramValue>> histogramValues;
        std::unordered_map<std::shared_ptr<Statistics::HistogramValue>, int> histogramValueIds;
        ObjectHandlerAdmin* admin = nullptr;
        ObjectType objectType;

        std::mutex mtx;
    public:

        void SetAdmin(ObjectHandlerAdmin* admin, ObjectType objectType)
        {
            this->admin = admin;
            this->objectType = objectType;
        }

        int Create(std::shared_ptr<Statistics::HistogramValue> value)
        {
            int id = admin->GetNewId();

            histogramValues[id] = std::make_shared<Statistics::HistogramValue>();
            histogramValueIds[histogramValues[id]] = id;

            admin->RegisterType(id, objectType);

            return id;
        }

        void Destroy(int id)
        {
            histogramValueIds.erase(histogramValues[id]);
            histogramValues.erase(id);

            admin->Remove(id);
        }

        std::shared_ptr<Statistics::HistogramValue> GetObject(int id)
        {
            return histogramValues[id];
        }

        int GetObjectId(const std::shared_ptr<Statistics::HistogramValue>& histogramValue, int newId)
        {
            if (!histogramValueIds.contains(histogramValue))
            {
                std::lock_guard lock(mtx);

                histogramValues[newId] = histogramValue;
                histogramValueIds[histogramValue] = newId;

                admin->RegisterType(newId, objectType);
            }

            return histogramValueIds[histogramValue];
        }
    };
}

