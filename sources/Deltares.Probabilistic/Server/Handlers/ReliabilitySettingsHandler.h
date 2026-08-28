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
#include "StochastSettingsHandler.h"
#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Reliability/Settings.h"
#include "../../Reliability/DesignPoint.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ReliabilitySettings
     */
    class ReliabilitySettingsHandler : public StoredObjectHandler<Reliability::Settings>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::Settings;
        }

        double GetValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_) override
        {
            if (property_ == "relaxation_factor") return settings->RelaxationFactor;
            else if (property_ == "variation_coefficient") return settings->VariationCoefficient;
            else if (property_ == "variance_factor") return settings->VarianceFactor;
            else if (property_ == "fraction_failed") return settings->FractionFailed;
            else if (property_ == "epsilon_beta") return settings->EpsilonBeta;
            else if (property_ == "epsilon_weight_sample") return settings->EpsilonWeightSample;
            else if (property_ == "epsilon_u_step_size") return settings->DirectionSettings->EpsilonUStepSize;
            else if (property_ == "epsilon_z_step_size") return settings->DirectionSettings->EpsilonZStepSize;
            else if (property_ == "dsdu") return settings->DirectionSettings->Dsdu;
            else if (property_ == "maximum_length_u") return settings->DirectionSettings->MaximumLengthU;
            else if (property_ == "maximum_length_start_point") return settings->StartPointSettings->MaximumLengthStartPoint;
            else if (property_ == "radius_sphere_search") return settings->StartPointSettings->RadiusSphereSearch;
            else if (property_ == "markov_chain_deviation") return settings->MarkovChainDeviation;
            else if (property_ == "subset_fraction") return settings->SubsetFraction;
            else if (property_ == "step_size") return settings->GradientSettings->StepSize;
            else if (property_ == "fragility_curve_step_size") return settings->FragilityCurveStepSize;
            else if (property_ == "start_value_step_size") return settings->StartValueStepSize;
            else if (property_ == "loop_variance_increment") return settings->LoopVarianceIncrement;
            else if (property_ == "max_beta") return settings->MaxBeta;
            else return StoredObjectHandler::GetValue(settings, property_);
        }

        void SetValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_, double value) override
        {
            if (property_ == "relaxation_factor") settings->RelaxationFactor = value;
            else if (property_ == "variation_coefficient") settings->VariationCoefficient = value;
            else if (property_ == "variance_factor") settings->VarianceFactor = value;
            else if (property_ == "fraction_failed") settings->FractionFailed = value;
            else if (property_ == "epsilon_weight_sample") settings->EpsilonWeightSample = value;
            else if (property_ == "epsilon_beta") settings->EpsilonBeta = value;
            else if (property_ == "epsilon_u_step_size") settings->DirectionSettings->EpsilonUStepSize = value;
            else if (property_ == "epsilon_z_step_size") settings->DirectionSettings->EpsilonZStepSize = value;
            else if (property_ == "dsdu") settings->DirectionSettings->Dsdu = value;
            else if (property_ == "maximum_length_u") settings->DirectionSettings->MaximumLengthU = value;
            else if (property_ == "maximum_length_start_point") settings->StartPointSettings->MaximumLengthStartPoint = value;
            else if (property_ == "radius_sphere_search") settings->StartPointSettings->RadiusSphereSearch = value;
            else if (property_ == "markov_chain_deviation") settings->MarkovChainDeviation = value;
            else if (property_ == "subset_fraction") settings->SubsetFraction = value;
            else if (property_ == "step_size") settings->GradientSettings->StepSize = value;
            else if (property_ == "fragility_curve_step_size") settings->FragilityCurveStepSize = value;
            else if (property_ == "start_value_step_size") settings->StartValueStepSize = value;
            else if (property_ == "loop_variance_increment") settings->LoopVarianceIncrement = value;
            else if (property_ == "max_beta") settings->MaxBeta = value;
            else StoredObjectHandler::SetValue(settings, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_) override
        {
            if (property_ == "minimum_samples") return settings->MinimumSamples;
            else if (property_ == "maximum_samples") return settings->MaximumSamples;
            else if (property_ == "maximum_samples_no_result") return settings->MaximumSamplesNoResult;
            else if (property_ == "minimum_iterations") return settings->MinimumIterations;
            else if (property_ == "maximum_iterations") return settings->MaximumIterations;
            else if (property_ == "minimum_directions") return settings->MinimumDirections;
            else if (property_ == "maximum_directions") return settings->MaximumDirections;
            else if (property_ == "minimum_variance_loops") return settings->MinimumVarianceLoops;
            else if (property_ == "maximum_variance_loops") return settings->MaximumVarianceLoops;
            else if (property_ == "minimum_failed_samples") return settings->MinimumFailedSamples;
            else if (property_ == "stochast_settings_count") return static_cast<int>(settings->StochastSet->stochastSettings.size());
            else if (property_ == "random_seed") return settings->RandomSettings->Seed;
            else if (property_ == "relaxation_loops") return settings->RelaxationLoops;
            else if (property_ == "max_steps_sphere_search") return settings->StartPointSettings->maxStepsSphereSearch;
            else if (property_ == "max_clusters") return settings->MaxClusters;
            else return modelProjectSettingsHandler->GetIntValue(settings, property_);
        }

        void SetIntValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_, int value) override
        {
            if (property_ == "minimum_samples") settings->MinimumSamples = value;
            else if (property_ == "maximum_samples") settings->MaximumSamples = value;
            else if (property_ == "maximum_samples_no_result") settings->MaximumSamplesNoResult = value;
            else if (property_ == "minimum_iterations") settings->MinimumIterations = value;
            else if (property_ == "maximum_iterations") settings->MaximumIterations = value;
            else if (property_ == "minimum_directions") settings->MinimumDirections = value;
            else if (property_ == "maximum_directions") settings->MaximumDirections = value;
            else if (property_ == "minimum_variance_loops") settings->MinimumVarianceLoops = value;
            else if (property_ == "maximum_variance_loops") settings->MaximumVarianceLoops = value;
            else if (property_ == "minimum_failed_samples") settings->MinimumFailedSamples = value;
            else if (property_ == "random_seed") settings->RandomSettings->Seed = value;
            else if (property_ == "max_clusters") settings->MaxClusters = value;
            else if (property_ == "relaxation_loops") settings->RelaxationLoops = value;
            else if (property_ == "max_steps_sphere_search") settings->StartPointSettings->maxStepsSphereSearch = value;
            else if (property_ == "start_point")
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPointCallback(value);
                settings->StochastSet->setStartPoint(designPoint->getSample());
            }
            else modelProjectSettingsHandler->SetIntValue(settings, property_, value);
        }

        int GetIdValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_) override
        {
            return modelProjectSettingsHandler->GetIdValue(settings, property_);
        }

        bool GetBoolValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_) override
        {
            if (property_ == "all_quadrants") return settings->StartPointSettings->allQuadrants;
            else if (property_ == "is_repeatable_random") return settings->RandomSettings->IsRepeatableRandom;
            else if (property_ == "filter_at_non_convergence") return settings->FilterAtNonConvergence;
            else if (property_ == "clustering") return settings->Clustering;
            else if (property_ == "optimize_number_clusters") return settings->OptimizeNumberOfClusters;
            else if (property_ == "auto_maximum_samples") return settings->AutoMaximumSamples;
            else if (property_ == "start_point_on_limit_state") return settings->StartPointOnLimitState;
            else return modelProjectSettingsHandler->GetBoolValue(settings, property_);
        }

        void SetBoolValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_, bool value) override
        {
            if (property_ == "all_quadrants") settings->StartPointSettings->allQuadrants = value;
            else if (property_ == "is_repeatable_random") settings->RandomSettings->IsRepeatableRandom = value;
            else if (property_ == "filter_at_non_convergence") settings->FilterAtNonConvergence = value;
            else if (property_ == "clustering") settings->Clustering = value;
            else if (property_ == "optimize_number_clusters") settings->OptimizeNumberOfClusters = value;
            else if (property_ == "auto_maximum_samples") settings->AutoMaximumSamples = value;
            else if (property_ == "start_point_on_limit_state") settings->StartPointOnLimitState = value;
            else modelProjectSettingsHandler->SetBoolValue(settings, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_) override
        {
            if (property_ == "reliability_method") return Reliability::Settings::getReliabilityMethodTypeString(settings->ReliabilityMethod);
            else if (property_ == "handle_invalid_type") return Models::RunSettings::getHandleInvalidTypeString(settings->RunSettings->handleInvalidType);
            else if (property_ == "reliability_result") return Reliability::Settings::getReliabilityResultTypeString(settings->ReliabilityResult);
            else if (property_ == "model_return_type") return Models::RunSettings::getModelReturnTypeString(settings->RunSettings->modelReturnType);
            else if (property_ == "design_point_method") return Reliability::DesignPointBuilder::getDesignPointMethodString(settings->designPointMethod);
            else if (property_ == "fragility_curve_design_point_method") return Reliability::DesignPointBuilder::getDesignPointMethodString(settings->fragilityCurveDesignPointMethod);
            else if (property_ == "sample_method") return Reliability::SubsetSimulationSettings::getSampleMethodString(settings->sampleMethod);
            else if (property_ == "start_method") return Reliability::StartPointCalculatorSettings::getStartPointMethodString(settings->StartPointSettings->StartMethod);
            else if (property_ == "gradient_type") return Models::GradientSettings::getGradientTypeString(settings->GradientSettings->gradientType);
            else if (property_ == "model_varying_type") return Reliability::DirectionReliabilitySettings::getModelVaryingTypeString(settings->DirectionSettings->modelVaryingType);
            else return modelProjectSettingsHandler->GetStringValue(settings, property_);
        }

        void SetStringValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_, const std::string& value) override
        {
            if (property_ == "reliability_method") settings->ReliabilityMethod = Reliability::Settings::getReliabilityMethodType(value);
            else if (property_ == "reliability_result") settings->ReliabilityResult = Reliability::Settings::getReliabilityResultType(value);
            else if (property_ == "handle_invalid_type") settings->RunSettings->handleInvalidType = Models::RunSettings::getHandleInvalidType(value);
            else if (property_ == "model_return_type") settings->RunSettings->modelReturnType = Models::RunSettings::getModelReturnType(value);
            else if (property_ == "design_point_method") settings->designPointMethod = Reliability::DesignPointBuilder::getDesignPointMethod(value);
            else if (property_ == "fragility_curve_design_point_method") settings->fragilityCurveDesignPointMethod = Reliability::DesignPointBuilder::getDesignPointMethod(value);
            else if (property_ == "sample_method") settings->sampleMethod = Reliability::SubsetSimulationSettings::getSampleMethod(value);
            else if (property_ == "start_method") settings->StartPointSettings->StartMethod = Reliability::StartPointCalculatorSettings::getStartPointMethod(value);
            else if (property_ == "gradient_type") settings->GradientSettings->gradientType = Models::GradientSettings::getGradientType(value);
            else if (property_ == "model_varying_type") settings->DirectionSettings->modelVaryingType = Reliability::DirectionReliabilitySettings::getModelVaryingType(value);
            else modelProjectSettingsHandler->SetStringValue(settings, property_, value);
        }

        int GetIndexedIdValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_, int index) override
        {
            if (property_ == "stochast_settings") return stochastSettingsHandler->GetObjectId(settings->StochastSet->stochastSettings[index]);
            else return StoredObjectHandler::GetIndexedIdValue(settings, property_, index);
        }

        void SetArrayIntValue(const std::shared_ptr<Reliability::Settings>& settings, const std::string& property_, int* values, int size) override
        {
            if (property_ == "stochast_settings")
            {
                settings->StochastSet->stochastSettings.clear();
                for (int i = 0; i < size; i++)
                {
                    settings->StochastSet->stochastSettings.push_back(stochastSettingsHandler->GetObject(values[i]));
                }
            }
            else
            {
                StoredObjectHandler::SetArrayIntValue(settings, property_, values, size);
            }
        }

        ModelProjectSettingsHandler* modelProjectSettingsHandler = nullptr;
        StochastSettingsHandler* stochastSettingsHandler = nullptr;
        GetObjectCallBack<Reliability::DesignPoint> designPointCallback = nullptr;
    };
}

