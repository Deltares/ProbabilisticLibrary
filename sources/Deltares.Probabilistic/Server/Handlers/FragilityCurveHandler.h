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

#include "DerivedObjectHandler.h"
#include "../../Reliability/FragilityCurve.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class FragilityCurve
     */
    class FragilityCurveHandler : public DerivedObjectHandler<Reliability::FragilityCurve, Statistics::Stochast>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::FragilityCurve;
        }

        double GetValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_) override
        {
            if (property_ == "fixed_value") return fragilityCurve->fixedValue;
            else return DerivedObjectHandler::GetValue(fragilityCurve, property_);
        }

        void SetValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_, double value) override
        {
            if (property_ == "fixed_value") fragilityCurve->fixedValue = value;
            else if (property_ == "design_point_x") argValue = value;
            else DerivedObjectHandler::SetValue(fragilityCurve, property_, value);
        }

        int GetIdValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_) override
        {
            if (property_ == "design_point_x")
            {
                double x = argValue;
                argValue = std::nan("");

                std::shared_ptr<Models::StochastPoint> stochastPoint = fragilityCurve->getDesignPoint(x);
                std::shared_ptr<Reliability::DesignPoint> designPoint = std::make_shared<Reliability::DesignPoint>(*stochastPoint);
                return designPointIdCallback(designPoint);
            }
            else return DerivedObjectHandler::GetIdValue(fragilityCurve, property_);
        }

        bool GetBoolValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_) override
        {
            if (property_ == "inverted") return fragilityCurve->inverted;
            else if (property_ == "fixed") return fragilityCurve->fixed;
            else return DerivedObjectHandler::GetBoolValue(fragilityCurve, property_);
        }

        void SetBoolValue(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, const std::string& property_, bool value) override
        {
            if (property_ == "inverted") fragilityCurve->inverted = value;
            else if (property_ == "fixed") fragilityCurve->fixed = value;
            else DerivedObjectHandler::SetBoolValue(fragilityCurve, property_, value);
        }

        GetObjectIdCallBack<Reliability::DesignPoint> designPointIdCallback = nullptr;

    private:
        double argValue = nan("");
    };
}

