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

#include "ModelProjectSettingsHandler.h"
#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Uncertainty/SettingsS.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class UncertaintySettings
     */
    class UncertaintySettingsHandler : public StoredObjectHandler<Uncertainty::SettingsS>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::UncertaintySettings;
        }

        double GetValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_) override
        {
            if (property_ == "variation_coefficient") return settings->VariationCoefficient;
            else if (property_ == "probability_for_convergence") return settings->ProbabilityForConvergence;
            else if (property_ == "minimum_u") return settings->MinimumU;
            else if (property_ == "maximum_u") return settings->MaximumU;
            else if (property_ == "step_size") return settings->GradientSettings->StepSize;
            else if (property_ == "step_size_factor") return settings->StepSizeFactor;
            else if (property_ == "global_step_size") return settings->GlobalStepSize;
            else return StoredObjectHandler::GetValue(settings, property_);
        }

        void SetValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_, double value) override
        {
            if (property_ == "variation_coefficient") settings->VariationCoefficient = value;
            else if (property_ == "probability_for_convergence") settings->ProbabilityForConvergence = value;
            else if (property_ == "minimum_u") settings->MinimumU = value;
            else if (property_ == "maximum_u") settings->MaximumU = value;
            else if (property_ == "step_size") settings->GradientSettings->StepSize = value;
            else if (property_ == "step_size_factor") settings->StepSizeFactor = value;
            else if (property_ == "global_step_size") settings->GlobalStepSize = value;
            else StoredObjectHandler::SetValue(settings, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_) override
        {
            if (property_ == "minimum_samples") return settings->MinimumSamples;
            else if (property_ == "maximum_samples") return settings->MaximumSamples;
            else if (property_ == "maximum_iterations") return settings->MaximumIterations;
            else if (property_ == "minimum_directions") return settings->MinimumDirections;
            else if (property_ == "maximum_directions") return settings->MaximumDirections;
            else if (property_ == "random_seed") return settings->RandomSettings->Seed;
            else if (property_ == "required_samples")
                return Uncertainty::CrudeMonteCarloSettingsS::getRequiredSamples(settings->ProbabilityForConvergence, settings->VariationCoefficient);
            else if (property_ == "quantiles_count") return static_cast<int>(settings->RequestedQuantiles.size());
            else if (property_ == "stochast_settings_count") return static_cast<int>(settings->StochastSet->stochastSettings.size());
            else return modelProjectSettingsHandler->GetIntValue(settings, property_);
        }

        void SetIntValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_, int value) override
        {
            if (property_ == "minimum_samples") settings->MinimumSamples = value;
            else if (property_ == "maximum_samples") settings->MaximumSamples = value;
            else if (property_ == "maximum_iterations") settings->MaximumIterations = value;
            else if (property_ == "minimum_directions") settings->MinimumDirections = value;
            else if (property_ == "maximum_directions") settings->MaximumDirections = value;
            else if (property_ == "random_seed") settings->RandomSettings->Seed = value;
            else modelProjectSettingsHandler->SetIntValue(settings, property_, value);
        }

        int GetIdValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_) override
        {
            return modelProjectSettingsHandler->GetIdValue(settings, property_);
        }

        bool GetBoolValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_) override
        {
            if (property_ == "derive_samples_from_variation_coefficient") return settings->DeriveSamplesFromVariationCoefficient;
            else if (property_ == "calculate_correlations") return settings->CalculateCorrelations;
            else if (property_ == "calculate_input_correlations") return settings->CalculateInputCorrelations;
            else if (property_ == "is_repeatable_random") return settings->RandomSettings->IsRepeatableRandom;
            else return modelProjectSettingsHandler->GetBoolValue(settings, property_);
        }

        void SetBoolValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_, bool value) override
        {
            if (property_ == "derive_samples_from_variation_coefficient") settings->DeriveSamplesFromVariationCoefficient = value;
            else if (property_ == "calculate_correlations") settings->CalculateCorrelations = value;
            else if (property_ == "calculate_input_correlations") settings->CalculateInputCorrelations = value;
            else if (property_ == "is_repeatable_random") settings->RandomSettings->IsRepeatableRandom = value;
            else modelProjectSettingsHandler->SetBoolValue(settings, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_) override
        {
            if (property_ == "uncertainty_method") return Uncertainty::SettingsS::getUncertaintyMethodTypeString(settings->UncertaintyMethod);
            else if (property_ == "gradient_type") return Models::GradientSettings::getGradientTypeString(settings->GradientSettings->gradientType);
            else return modelProjectSettingsHandler->GetStringValue(settings, property_);
        }

        void SetStringValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_, const std::string& value) override
        {
            if (property_ == "uncertainty_method") settings->UncertaintyMethod = Uncertainty::SettingsS::getUncertaintyMethodType(value);
            else if (property_ == "gradient_type") settings->GradientSettings->gradientType = Models::GradientSettings::getGradientType(value);
            else modelProjectSettingsHandler->SetStringValue(settings, property_, value);
        }

        void SetArrayIntValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_, int* values, int size) override
        {
            if (property_ == "stochast_settings")
            {
                settings->StochastSet->stochastSettings.clear();
                for (int i = 0; i < size; i++)
                {
                    settings->StochastSet->stochastSettings.push_back(stochastSettingsHandler->GetObject(values[i]));
                }
            }
            else if (property_ == "quantiles")
            {
                settings->RequestedQuantiles.clear();
                for (int i = 0; i < size; i++)
                {
                    settings->RequestedQuantiles.push_back(probabilityValueHandler->GetObject(values[i]));
                }
            }
            else
            {
                StoredObjectHandler::SetArrayIntValue(settings, property_, values, size);
            }
        }

        int GetIndexedIdValue(const std::shared_ptr<Uncertainty::SettingsS>& settings, const std::string& property_, int index) override
        {
            if (property_ == "quantiles") return probabilityValueHandler->GetObjectId(settings->RequestedQuantiles[index]);
            else if (property_ == "stochast_settings") return stochastSettingsHandler->GetObjectId(settings->StochastSet->stochastSettings[index]);
            else return StoredObjectHandler::GetIndexedIdValue(settings, property_, index);
        }

        ModelProjectSettingsHandler* modelProjectSettingsHandler = nullptr;
        StochastSettingsHandler* stochastSettingsHandler = nullptr;
        ProbabilityValueHandler* probabilityValueHandler = nullptr;
    };
}

