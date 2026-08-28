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

#include "FragilityCurveSettingsHandler.h"
#include "SelfCorrelationMatrixHandler.h"
#include "StoredObjectHandler.h"
#include "../../Reliability/FragilityCurveProject.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class FragilityCurveProject
     */
    class FragilityCurveProjectHandler : public StoredObjectHandler<Reliability::FragilityCurveProject>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::FragilityCurveProject;
        }

        int GetIdValue(const std::shared_ptr<Reliability::FragilityCurveProject>& project, const std::string& property_) override
        {
            if (property_ == "design_point") return designPointHandler->GetObjectId(project->designPoint);
            else if (property_ == "integrand") return stochastHandler->GetObjectId(project->integrand);
            else if (property_ == "fragility_curve") return fragilityCurveHandler->GetObjectId(project->fragilityCurve);
            else if (property_ == "fragility_curve_normalized") return fragilityCurveHandler->GetObjectId(project->fragilityCurveNormalized);
            else if (property_ == "settings") return fragilityCurveSettingsHandler->GetObjectId(project->settings);
            else return StoredObjectHandler::GetIdValue(project, property_);
        }

        void SetIntValue(const std::shared_ptr<Reliability::FragilityCurveProject>& project, const std::string& property_, int value) override
        {
            if (property_ == "integrand") project->integrand = stochastHandler->GetObject(value);
            else if (property_ == "fragility_curve") project->fragilityCurve = fragilityCurveHandler->GetObject(value);
            else if (property_ == "fragility_curve_normalized") project->fragilityCurveNormalized = fragilityCurveHandler->GetObject(value);
            else if (property_ == "settings") project->settings = fragilityCurveSettingsHandler->GetObject(value);
            else StoredObjectHandler::SetIntValue(project, property_, value);
        }

        void Execute(const std::shared_ptr<Reliability::FragilityCurveProject>& project, const std::string& method_) override
        {
            if (method_ == "run") project->run();
            else StoredObjectHandler::Execute(project, method_);
        }

        DesignPointHandler* designPointHandler = nullptr;
        StochastHandler* stochastHandler = nullptr;
        FragilityCurveHandler* fragilityCurveHandler = nullptr;
        FragilityCurveSettingsHandler* fragilityCurveSettingsHandler = nullptr;
    };
}

