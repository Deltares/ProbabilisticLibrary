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

#include "../ProjectEntries.h"
#include "DiscreteValueHandler.h"
#include "FragilityValueHandler.h"
#include "HistogramValueHandler.h"
#include "MessageHandler.h"

namespace Deltares::Server
{
    class HandlerAdmin
    {
    public:
        HandlerAdmin();

        HistogramValueHandler histogramValueHandler;
        DiscreteValueHandler discreteValueHandler;
        FragilityValueHandler fragilityValueHandler;
        MessageHandler messageHandler;

        int GetNewId()
        {
            return admin.GetNewId();
        }

        bool IsSupported(ObjectType objectType);

        void RegisterType(int id, ObjectType objectType)
        {
            admin.RegisterType(id, objectType);
        }

        ObjectType GetObjectType(int id)
        {
            return admin.GetObjectType(id);
        }

        int Create(ObjectType objectType)
        {
            return handlers[objectType]->Create();
        }

        bool Contains(int id)
        {
            return admin.Contains(id);
        }

        void Destroy(int id)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->Destroy(id);
        }

        void Remove(int id)
        {
            admin.Remove(id);
        }

        bool IsEmpty()
        {
            return admin.IsEmpty();
        }

        int GetSize() const
        {
            return admin.GetSize();
        }

        double GetValue(int id, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetValue(id, property_);
        }

        void SetValue(int id, const std::string& property_, double value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetValue(id, property_, value);
        }

        std::string GetStringValue(int id, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetStringValue(id, property_);
        }

        void SetStringValue(int id, const std::string& property_, std::string value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetStringValue(id, property_, value);
        }

        int GetIntValue(int id, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetIntValue(id, property_);
        }

        int GetIdValue(int id, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetIdValue(id, property_);
        }

        void SetIntValue(int id, const std::string& property_, int value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetIntValue(id, property_, value);
        }

        bool GetBoolValue(int id, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetBoolValue(id, property_);
        }

        void SetBoolValue(int id, const std::string& property_, bool value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetBoolValue(id, property_, value);
        }

        double GetIntArgValue(int id1, int id2, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id1);
            return handlers[objectType]->GetIntArgValue(id1, id2, property_);
        }

    private:

        ObjectHandlerAdmin admin;
        std::unordered_map<ObjectType, ObjectHandler*> handlers;

    };
}
