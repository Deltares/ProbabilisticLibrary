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
#include <unordered_map>
#include "../ProjectEntries.h"

namespace Deltares::Server
{
    class ObjectHandlerAdmin
    {
    private:
        int new_id = 0;
        std::mutex mtx;

        std::unordered_map<int, ObjectType> types;

    public:

        int GetNewId()
        {
            std::lock_guard lock(mtx);
            new_id++;

            return new_id;
        }

        void RegisterType(int id, ObjectType objectType)
        {
            std::lock_guard lock(mtx);

            types[id] = objectType;
        }

        ObjectType GetObjectType(int id)
        {
            return types[id];
        }

        void Remove(int id)
        {
            std::lock_guard lock(mtx);

            types.erase(id);
        }

        bool Contains(int id)
        {
            const auto it = types.find(id);
            if (it == types.end()) return false;

            return true;
        }

        bool IsEmpty() const
        {
            return types.empty();
        }

        int GetSize() const
        {
            return static_cast<int>(types.size());
        }
    };
}

