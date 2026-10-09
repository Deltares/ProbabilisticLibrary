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
#include "SearchParameterSettingsHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Optimization/OptimizationSettings.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class SensitivitySettings
     */
    class OptimizationSettingsHandler : public DerivedObjectHandler<Optimization::OptimizationSettings, Models::ModelProjectSettings>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::OptimizationSettings;
        }

        double GetValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_) override
        {
            if (property_ == "epsilon_beta") return settings->EpsilonBeta;
            else return DerivedObjectHandler::GetValue(settings, property_);
        }

        void SetValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_, double value) override
        {
            if (property_ == "epsilon_beta") settings->EpsilonBeta = value;
            else DerivedObjectHandler::SetValue(settings, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_) override
        {
            if (property_ == "maximum_iterations") return settings->Iterations;
            else if (property_ == "max_grid_moves") return settings->MaxGridMoves;
            else if (property_ == "search_parameter_settings_count") return static_cast<int>(settings->SearchArea->Dimensions.size());
            else return DerivedObjectHandler::GetIntValue(settings, property_);
        }

        void SetIntValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_, int value) override
        {
            if (property_ == "maximum_iterations") settings->Iterations = value;
            else if (property_ == "max_grid_moves") settings->MaxGridMoves = value;
            else DerivedObjectHandler::SetIntValue(settings, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_) override
        {
            if (property_ == "optimization_method") return Optimization::OptimizationSettings::getOptimizationMethodTypeString(settings->OptimizationMethod);
            else return DerivedObjectHandler::GetStringValue(settings, property_);
        }

        void SetStringValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_, const std::string& value) override
        {
            if (property_ == "optimization_method") settings->OptimizationMethod = Optimization::OptimizationSettings::getOptimizationMethodType(value);
            else DerivedObjectHandler::SetStringValue(settings, property_, value);
        }

        int GetIndexedIdValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_, int index) override
        {
            if (property_ == "search_parameter_settings") return searchParameterSettingsHandler->GetObjectId(settings->SearchArea->Dimensions[index]);
            else return DerivedObjectHandler::GetIndexedIdValue(settings, property_, index);
        }

        void SetArrayIntValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_, int* values, int size) override
        {
            if (property_ == "search_parameter_settings") searchParameterSettingsHandler->SetIdValues(settings->SearchArea->Dimensions, values, size);
            else DerivedObjectHandler::SetArrayIntValue(settings, property_, values, size);
        }

        SearchParameterSettingsHandler* searchParameterSettingsHandler = nullptr;
    };
}

