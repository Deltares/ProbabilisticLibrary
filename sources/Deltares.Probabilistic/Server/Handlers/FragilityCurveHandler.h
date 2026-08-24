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
#include "../../Reliability/FragilityCurve.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class FragilityCurve
     */
    class FragilityCurveHandler : public StoredObjectHandler<Reliability::FragilityCurve>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::FragilityCurve;
        }

        double GetValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = GetObject(id);

            if (property_ == "fixed_value") return fragilityCurve->fixedValue;
            else return ObjectHandler::GetValue(id, property_);
        }

        void SetValue(int id, const std::string& property_, double value) override
        {
            std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = GetObject(id);

            if (property_ == "fixed_value") fragilityCurve->fixedValue = value;
            else ObjectHandler::SetValue(id, property_, value);
        }

        int GetIdValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = GetObject(id);

            if (property_ == "design_point_x")
            {
                double x = argValue;
                argValue = std::nan("");

                std::shared_ptr<Models::StochastPoint> stochastPoint = fragilityCurve->getDesignPoint(x);
                std::shared_ptr<Reliability::DesignPoint> designPoint = std::make_shared<DesignPoint>(*stochastPoint);

                return GetDesignPointId(designPoint, newId);
            }
            else return ObjectHandler::GetIdValue(id, property_);
        }

        void SetIntValue(int id, const std::string& property_, int value) override
        {
            std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = GetObject(id);

            if (property_ == "index") modelParameter->index = value;
            else if (property_ == "array_size") modelParameter->arraySize = value;
            else ObjectHandler::SetIntValue(id, property_, value);
        }

        bool GetBoolValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = GetObject(id);

            if (property_ == "fixed") return fragilityCurve->fixed;
            else return ObjectHandler::GetBoolValue(id, property_);
        }

        void SetBoolValue(int id, const std::string& property_, bool value) override
        {
            std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = GetObject(id);

            if (property_ == "is_array") modelParameter->isArray = value;
            else ObjectHandler::SetBoolValue(id, property_, value);
        }

        std::string GetStringValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = GetObject(id);

            if (property_ == "name") return modelParameter->name;
            else return ObjectHandler::GetStringValue(id, property_);
        }

        void SetStringValue(int id, const std::string& property_, const std::string& value) override
        {
            std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = GetObject(id);

            if (property_ == "name") modelParameter->name = value;
            else ObjectHandler::SetStringValue(id, property_, value);
        }
    private:
        double argValue = nan("");
    };
}

