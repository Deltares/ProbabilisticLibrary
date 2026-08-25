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

#include "StochastHandler.h"
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

        double GetValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_) override
        {
            if (property_ == "fixed_value") return fragilityCurve->fixedValue;
            else return stochastHandler->GetValue(fragilityCurve, property_);
        }

        void SetValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_, double value) override
        {
            if (property_ == "fixed_value") fragilityCurve->fixedValue = value;
            else stochastHandler->SetValue(fragilityCurve, property_, value);
        }

        int GetIdValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_) override
        {
            if (property_ == "design_point_x")
            {
                double x = argValue;
                argValue = std::nan("");

                std::shared_ptr<Models::StochastPoint> stochastPoint = fragilityCurve->getDesignPoint(x);

                //std::shared_ptr<Reliability::DesignPoint> designPoint = std::make_shared<DesignPoint>(*stochastPoint);
                //return GetDesignPointId(designPoint, newId);

                // TODO
                return 0;
            }
            else stochastHandler->GetIdValue(fragilityCurve, property_);
        }

        int GetIndexedIdValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_, int index) override
        {
            return stochastHandler->GetIndexedIdValue(fragilityCurve, property_, index);
        }

        int GetIntValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_) override
        {
            return stochastHandler->GetIntValue(fragilityCurve, property_);
        }

        void SetIntValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_, int value) override
        {
            stochastHandler->SetIntValue(fragilityCurve, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_) override
        {
            if (property_ == "inverted") return fragilityCurve->inverted;
            else if (property_ == "fixed") return fragilityCurve->fixed;
            else return stochastHandler->GetBoolValue(fragilityCurve, property_);
        }

        void SetBoolValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_, bool value) override
        {
            if (property_ == "inverted") fragilityCurve->inverted = value;
            else if (property_ == "fixed") fragilityCurve->fixed = value;
            else stochastHandler->SetBoolValue(fragilityCurve, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_) override
        {
            return stochastHandler->GetStringValue(fragilityCurve, property_);
        }

        void SetStringValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_, const std::string& value) override
        {
            stochastHandler->SetStringValue(fragilityCurve, property_, value);
        }

        void SetArrayValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_, double* values, int size) override
        {
            return stochastHandler->SetArrayValue(fragilityCurve, property_, values, size);
        }

        double GetArgValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_, double argument) override
        {
            return stochastHandler->GetArgValue(fragilityCurve, property_, argument);
        }

        void SetArgValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_, double argument, double value) override
        {
            stochastHandler->SetArgValue(fragilityCurve, property_, argument, value);
        }

        StochastHandler* stochastHandler = nullptr;

    private:
        double argValue = nan("");
    };
}

