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
     * \brief Handles properties and methods of class OptimizationSettings
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
            else if (property_ == "differential_weight") return settings->DifferentialWeight;
            else if (property_ == "cross_over") return settings->CrossOver;
            else if (property_ == "beta") return settings->Beta;
            else if (property_ == "delta") return settings->Delta;
            else if (property_ == "mutation_rate") return settings->MutationRate;
            else if (property_ == "creep_reduction") return settings->CreepReduction;
            else return DerivedObjectHandler::GetValue(settings, property_);
        }

        void SetValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_, double value) override
        {
            if (property_ == "epsilon_beta") settings->EpsilonBeta = value;
            else if (property_ == "differential_weight") settings->DifferentialWeight = value;
            else if (property_ == "cross_over") settings->CrossOver = value;
            else if (property_ == "beta") settings->Beta = value;
            else if (property_ == "delta") settings->Delta = value;
            else if (property_ == "mutation_rate") settings->MutationRate = value;
            else if (property_ == "creep_reduction") settings->CreepReduction = value;
            else DerivedObjectHandler::SetValue(settings, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_) override
        {
            if (property_ == "maximum_iterations") return settings->Iterations;
            else if (property_ == "max_grid_moves") return settings->MaxGridMoves;
            else if (property_ == "search_parameter_settings_count") return static_cast<int>(settings->SearchArea->Dimensions.size());
            else if (property_ == "generation_count") return settings->GenerationCount;
            else if (property_ == "elite_count") return settings->EliteCount;
            else if (property_ == "population_count") return settings->PopulationCount;
            else if (property_ == "stop_after_non_improving_generations") return settings->StopAfterNonImprovingGenerations;
            else if (property_ == "seed") return settings->random_settings.Seed;
            else if (property_ == "cross_over_fractions_count" || property_ == "mutation_fractions_count") return 3;
            else return DerivedObjectHandler::GetIntValue(settings, property_);
        }

        void SetIntValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_, int value) override
        {
            if (property_ == "maximum_iterations") settings->Iterations = value;
            else if (property_ == "max_grid_moves") settings->MaxGridMoves = value;
            else if (property_ == "add_search_parameter_settings") settings->SearchArea->Dimensions.push_back(searchParameterSettingsHandler->GetObject(value));
            else if (property_ == "generation_count") settings->GenerationCount = value;
            else if (property_ == "elite_count") settings->EliteCount = value;
            else if (property_ == "population_count") settings->PopulationCount = value;
            else if (property_ == "stop_after_non_improving_generations") settings->StopAfterNonImprovingGenerations = value;
            else if (property_ == "seed") settings->random_settings.Seed = value;
            else DerivedObjectHandler::SetIntValue(settings, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_) override
        {
            if (property_ == "optimization_method")
            {
                return Optimization::OptimizationSettings::getOptimizationMethodTypeString(settings->OptimizationMethod);
            }
            else if (property_ == "initialize_population_method")
            {
                return Optimization::OptimizationSettings::getInitializePopulationMethod(settings->initializePopulationMethod);
            }
            else if (property_ == "cross_over_random_frequency")
            {
                return Optimization::OptimizationSettings::getRandomFrequency(settings->CrossOverRandomFrequency);
            }
            else if (property_ == "mutation_random_frequency")
            {
                return Optimization::OptimizationSettings::getRandomFrequency(settings->MutationRandomFrequency);
            }
            else if (property_ == "cross_over_random_effect")
            {
                return Optimization::OptimizationSettings::getRandomEffectString(settings->CrossOverRandomEffect);
            }
            else if (property_ == "mutation_random_effect")
            {
                return Optimization::OptimizationSettings::getRandomEffectString(settings->MutationRandomEffect);
            }
            else return DerivedObjectHandler::GetStringValue(settings, property_);
        }

        void SetStringValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_, const std::string& value) override
        {
            if (property_ == "optimization_method")
            {
                settings->OptimizationMethod = Optimization::OptimizationSettings::getOptimizationMethodType(value);
            }
            else if (property_ == "initialize_population_method")
            {
                settings->initializePopulationMethod = Optimization::OptimizationSettings::getInitializePopulationMethod(value);
            }
            else if (property_ == "cross_over_random_frequency")
            {
                settings->CrossOverRandomFrequency = Optimization::OptimizationSettings::getRandomFrequencyString(value);
            }
            else if (property_ == "mutation_random_frequency")
            {
                settings->MutationRandomFrequency = Optimization::OptimizationSettings::getRandomFrequencyString(value);
            }
            else if (property_ == "cross_over_random_effect")
            {
                settings->CrossOverRandomEffect = Optimization::OptimizationSettings::getRandomEffect(value);
            }
            else if (property_ == "mutation_random_effect")
            {
                settings->MutationRandomEffect = Optimization::OptimizationSettings::getRandomEffect(value);
            }
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

        double GetIndexedValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_, int index) override
        {
            if (property_ == "cross_over_fractions")
            {
                double return_value;
                switch (index)
                {
                case 0: return_value = settings->ScatterFraction;  break;
                case 1: return_value = settings->SinglePointFraction; break;
                case 2: return_value = settings->DoublePointFraction; break;
                default: return_value = 0.0;
                }
                return return_value;
            }
            else if (property_ == "mutation_fractions")
            {
                double return_value;
                switch (index)
                {
                case 0: return_value = settings->JumpFraction;  break;
                case 1: return_value = settings->InverseFraction; break;
                case 2: return_value = settings->CreepFraction; break;
                default: return_value = 0.0;
                }
                return return_value;
            }
            else return DerivedObjectHandler::GetIndexedValue(settings, property_, index);
        }

        void SetArrayValue(const std::shared_ptr<Optimization::OptimizationSettings>& settings, const std::string& property_, double* values, int size) override
        {
            if (property_ == "cross_over_fractions")
            {
                if (size < 3) throw Reliability::ProbabilisticLibraryException("size of cross_over_fractions must be 3");
                settings->ScatterFraction = values[0];
                settings->SinglePointFraction = values[1];
                settings->DoublePointFraction = values[2];
            }
            else if (property_ == "mutation_fractions")
            {
                if (size < 3) throw Reliability::ProbabilisticLibraryException("size of mutation_fractions must be 3");
                settings->JumpFraction = values[0];
                settings->InverseFraction = values[1];
                settings->CreepFraction = values[2];
            }
            else DerivedObjectHandler::SetArrayValue(settings, property_, values, size);
        }

        SearchParameterSettingsHandler* searchParameterSettingsHandler = nullptr;
    };
}

