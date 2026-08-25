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
#include "ProjectHandler.h"

#include "../Reliability/ProbabilityLimitStateFunction.h"
#include "../Statistics/CopulaCorrelation.h"

namespace Deltares::Server
{
    using namespace Deltares::Statistics;
    using namespace Deltares::Reliability;

    ProjectHandler::ProjectHandler()
    {
    }

    bool ProjectHandler::CanHandle(const std::string& object_type)
    {
        return ProjectEntries::CanHandle(object_type);
    }

    int ProjectHandler::GetNewId()
    {
        return admin.GetNewId();
    }

    bool ProjectHandler::IsSupported(ObjectType objectType)
    {
        return admin.IsSupported(objectType);
    }

    int ProjectHandler::Create(const std::string& object_type)
    {
        ObjectType objectType = ProjectEntries::GetType(object_type);

        if (admin.IsSupported(objectType))
        {
            return admin.Create(objectType);
        }

        int id = this->GetNewId();

        std::lock_guard lock(mtx);

        admin.RegisterType(id, objectType);

        switch (objectType)
        {
        case ObjectType::StandardNormal:
            break; // nothing to do
        case ObjectType::Project:
            projects[id] = std::make_shared<ReliabilityProject>();
            break;
        case ObjectType::CorrelationMatrix:
            correlations[id] = std::make_shared<CorrelationMatrix>(true);
            correlationIds[correlations[id]] = id;
            break;
        case ObjectType::CopulaCorrelation:
            correlations[id] = std::make_shared<CopulaCorrelation>();
            correlationIds[correlations[id]] = id;
            break;
        case ObjectType::Settings:
            settingsValues[id] = std::make_shared<Settings>();
            settingsValuesIds[settingsValues[id]] = id;
            break;
        case ObjectType::StochastSettings:
            stochastSettingsValues[id] = std::make_shared<StochastSettings>();
            break;
        case ObjectType::FragilityCurveProject:
            fragilityCurveProjects[id] = std::make_shared<FragilityCurveProject>();
            break;
        case ObjectType::FragilityCurveSettings:
            fragilityCurveSettings[id] = std::make_shared<FragilityCurveIntegrationSettings>();
            break;
        case ObjectType::CombineProject:
            combineProjects[id] = std::make_shared<CombineProject>();
            break;
        case ObjectType::CombineSettings:
            combineSettingsValues[id] = std::make_shared<CombineSettings>();
            break;
        case ObjectType::ExcludingCombineProject:
            excludingCombineProjects[id] = std::make_shared<ExcludingCombineProject>();
            break;
        case ObjectType::ExcludingCombineSettings:
            excludingCombineSettings[id] = std::make_shared<ExcludingCombineSettings>();
            break;
        case ObjectType::SelfCorrelationMatrix:
            selfCorrelationMatrices[id] = std::make_shared<SelfCorrelationMatrix>();
            selfCorrelationIds[selfCorrelationMatrices[id]] = id;
            break;
        case ObjectType::RunProject:
            runProjects[id] = std::make_shared<Models::RunProject>();
            break;
        case ObjectType::RunProjectSettings:
            runProjectSettings[id] = std::make_shared<Models::RunProjectSettings>();
            break;
        case ObjectType::UncertaintyProject:
            uncertaintyProjects[id] = std::make_shared<Uncertainty::UncertaintyProject>();
            break;
        case ObjectType::UncertaintySettings:
            uncertaintySettingsValues[id] = std::make_shared<Uncertainty::SettingsS>();
            break;
        case ObjectType::UncertaintyResult:
            uncertaintyResults[id] = std::make_shared<Uncertainty::UncertaintyResult>();
            uncertaintyResultsIds[uncertaintyResults[id]] = id;
            break;
        case ObjectType::SensitivityProject:
            sensitivityProjects[id] = std::make_shared<Sensitivity::SensitivityProject>();
            break;
        case ObjectType::SensitivitySettings:
            sensitivitySettingsValues[id] = std::make_shared<Sensitivity::SensitivitySettings>();
            break;
        case ObjectType::SensitivityResult:
            sensitivityResults[id] = std::make_shared<Sensitivity::SensitivityResult>();
            sensitivityResultsIds[sensitivityResults[id]] = id;
            break;
        case ObjectType::SensitivityValue:
            sensitivityValues[id] = std::make_shared<Sensitivity::SensitivityValue>();
            sensitivityValuesIds[sensitivityValues[id]] = id;
            break;
        case ObjectType::LengthEffectProject:
            lengthEffectProjects[id] = std::make_shared<LengthEffectProject>();
            break;
        default: throw ProbabilisticLibraryException("object type");
        }

        return id;
    }

    void ProjectHandler::Destroy(int id)
    {
        std::lock_guard lock(mtx);

        if (!admin.Contains(id))
        {
            return;
        }

        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            admin.Destroy(id);
            return;
        }

        switch (objectType)
        {
        case ObjectType::Project: projects.erase(id); break;
        case ObjectType::CorrelationMatrix:
        case ObjectType::CopulaCorrelation:
            correlationIds.erase(correlations[id]); correlations.erase(id); break;
        case ObjectType::Settings: settingsValuesIds.erase(settingsValues[id]); settingsValues.erase(id); break;
        case ObjectType::StochastSettings: stochastSettingsValues.erase(id); break;
        case ObjectType::FragilityCurveProject: fragilityCurveProjects.erase(id); break;
        case ObjectType::FragilityCurveSettings: fragilityCurveSettings.erase(id); break;
        case ObjectType::CombineProject: combineProjects.erase(id); break;
        case ObjectType::CombineSettings: combineSettingsValues.erase(id); break;
        case ObjectType::ExcludingCombineProject: excludingCombineProjects.erase(id); break;
        case ObjectType::ExcludingCombineSettings: excludingCombineSettings.erase(id); break;
        case ObjectType::SelfCorrelationMatrix: selfCorrelationIds.erase(selfCorrelationMatrices[id]); selfCorrelationMatrices.erase(id); break;
        case ObjectType::RunProject: runProjects.erase(id); break;
        case ObjectType::RunProjectSettings: runProjectSettings.erase(id); break;
        case ObjectType::UncertaintyProject: uncertaintyProjects.erase(id); break;
        case ObjectType::UncertaintySettings: uncertaintySettingsValues.erase(id); break;
        case ObjectType::UncertaintyResult: uncertaintyResultsIds.erase(uncertaintyResults[id]); uncertaintyResults.erase(id); break;
        case ObjectType::SensitivityProject: sensitivityProjects.erase(id); break;
        case ObjectType::SensitivitySettings: sensitivitySettingsValues.erase(id); break;
        case ObjectType::SensitivityResult: sensitivityResultsIds.erase(sensitivityResults[id]); sensitivityResults.erase(id); break;
        case ObjectType::SensitivityValue: sensitivityValuesIds.erase(sensitivityValues[id]); sensitivityValues.erase(id); break;
        case ObjectType::LengthEffectProject: lengthEffectProjects.erase(id); break;
        default: throw ProbabilisticLibraryException("object type");
        }

        admin.Remove(id);
    }

    bool ProjectHandler::ShouldClose()
    {
        return admin.IsEmpty();
    }

    double ProjectHandler::GetValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.GetValue(id, property_);
        }

        if (objectType == ObjectType::Settings)
        {
            std::shared_ptr<Settings> settings = settingsValues[id];

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
        }
        else if (objectType == ObjectType::StochastSettings)
        {
            std::shared_ptr<StochastSettings> stochastSettings = stochastSettingsValues[id];

            if (property_ == "min_value") return stochastSettings->MinValue;
            else if (property_ == "max_value") return stochastSettings->MaxValue;
            else if (property_ == "start_value") return stochastSettings->StartValue;
            else if (property_ == "variance_factor") return stochastSettings->VarianceFactor;
        }
        else if (objectType == ObjectType::UncertaintySettings)
        {
            std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

            if (property_ == "variation_coefficient") return settings->VariationCoefficient;
            else if (property_ == "probability_for_convergence") return settings->ProbabilityForConvergence;
            else if (property_ == "minimum_u") return settings->MinimumU;
            else if (property_ == "maximum_u") return settings->MaximumU;
            else if (property_ == "step_size") return settings->GradientSettings->StepSize;
            else if (property_ == "step_size_factor") return settings->StepSizeFactor;
            else if (property_ == "global_step_size") return settings->GlobalStepSize;
        }
        else if (objectType == ObjectType::FragilityCurveSettings)
        {
            std::shared_ptr<FragilityCurveIntegrationSettings> settings = fragilityCurveSettings[id];

            if (property_ == "step_size") return settings->StepSize;
        }
        else if (objectType == ObjectType::SensitivitySettings)
        {
            std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

            if (property_ == "low_value") return settings->LowValue;
            else if (property_ == "high_value") return settings->HighValue;
        }
        else if (objectType == ObjectType::SensitivityValue)
        {
            std::shared_ptr<Sensitivity::SensitivityValue> sensitivity_value = sensitivityValues[id];

            if (property_ == "low") return sensitivity_value->low;
            else if (property_ == "medium") return sensitivity_value->medium;
            else if (property_ == "high") return sensitivity_value->high;
            else if (property_ == "first_order_index") return sensitivity_value->firstOrderIndex;
            else if (property_ == "total_index") return sensitivity_value->totalIndex;
        }
        else if (objectType == ObjectType::LengthEffectProject)
        {
            std::shared_ptr<LengthEffectProject> length_effect = lengthEffectProjects[id];

            if (property_ == "length") return length_effect->length;
        }
        return std::nan("");
    }

    void ProjectHandler::SetValue(int id, const std::string& property_, double value)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            admin.SetValue(id, property_, value);
        }

        if (objectType == ObjectType::Settings)
        {
            std::shared_ptr<Settings> settings = settingsValues[id];

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
        }
        else if (objectType == ObjectType::FragilityCurveSettings)
        {
            std::shared_ptr<FragilityCurveIntegrationSettings> settings = fragilityCurveSettings[id];

            if (property_ == "step_size") settings->StepSize = value;
        }
        else if (objectType == ObjectType::UncertaintySettings)
        {
            std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

            if (property_ == "variation_coefficient") settings->VariationCoefficient = value;
            else if (property_ == "probability_for_convergence") settings->ProbabilityForConvergence = value;
            else if (property_ == "minimum_u") settings->MinimumU = value;
            else if (property_ == "maximum_u") settings->MaximumU = value;
            else if (property_ == "step_size") settings->GradientSettings->StepSize = value;
            else if (property_ == "step_size_factor") settings->StepSizeFactor = value;
            else if (property_ == "global_step_size") settings->GlobalStepSize = value;
        }
        else if (objectType == ObjectType::SensitivitySettings)
        {
            std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

            if (property_ == "low_value") settings->LowValue = value;
            else if (property_ == "high_value") settings->HighValue = value;
        }
        else if (objectType == ObjectType::StochastSettings)
        {
            std::shared_ptr<StochastSettings> stochastSettings = stochastSettingsValues[id];

            if (property_ == "min_value") stochastSettings->MinValue = value;
            else if (property_ == "max_value") stochastSettings->MaxValue = value;
            else if (property_ == "start_value") stochastSettings->StartValue = value;
            else if (property_ == "variance_factor") stochastSettings->VarianceFactor = value;
        }
        else if (objectType == ObjectType::LengthEffectProject)
        {
            std::shared_ptr<LengthEffectProject> length_effect = lengthEffectProjects[id];

            if (property_ == "length") length_effect->length = value;
        }
        else if (objectType == ObjectType::SensitivityValue)
        {
            std::shared_ptr<Sensitivity::SensitivityValue> sensitivity_value = sensitivityValues[id];

            if (property_ == "low") sensitivity_value->low = value;
            else if (property_ == "medium") sensitivity_value->medium = value;
            else if (property_ == "high") sensitivity_value->high = value;
            else if (property_ == "first_order_index") sensitivity_value->firstOrderIndex = value;
            else if (property_ == "total_index") sensitivity_value->totalIndex = value;
        }
    }

    int ProjectHandler::GetIntValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.GetIntValue(id, property_);
        }

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "index") return project->model->Index;
            else if (property_ == "stochasts_count") return static_cast<int>(project->stochasts.size());
            else if (property_ == "total_model_runs") return project->modelRuns;
        }

        if (objectType == ObjectType::Project)
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "index") return project->model->Index;
            else if (property_ == "stochasts_count") return static_cast<int>(project->stochasts.size());
            else if (property_ == "total_model_runs") return project->modelRuns;
        }

        if (objectType == ObjectType::UncertaintyProject)
        {
            std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

            if (property_ == "uncertainty_stochasts_count") return static_cast<int>(project->uncertaintyResults.size());
            else if (property_ == "uncertainty_results_count") return static_cast<int>(project->uncertaintyResults.size());
            else if (property_ == "uncertainty_parameters_count") return static_cast<int>(project->uncertaintyParameters.size());
        }
        else if (objectType == ObjectType::SensitivityProject)
        {
            std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

            if (property_ == "results_count") return static_cast<int>(project->sensitivityResults.size());
            else if (property_ == "sensitivity_parameters_count") return static_cast<int>(project->sensitivityParameters.size());
        }
        else if (objectType == ObjectType::SensitivityResult)
        {
            std::shared_ptr<Sensitivity::SensitivityResult> result = sensitivityResults[id];

            if (property_ == "values_count") return static_cast<int>(result->values.size());
            else if (property_ == "evaluations_count") return static_cast<int>(result->evaluations.size());
            else if (property_ == "messages_count") return static_cast<int>(result->messages.size());
        }
        else if (objectType == ObjectType::CorrelationMatrix)
        {
            std::shared_ptr<CorrelationMatrix> matrix = std::dynamic_pointer_cast<CorrelationMatrix>(correlations[id]);

            if (property_ == "count_correlations") return matrix->CountCorrelations();
            else if (property_ == "variables_count") return matrix->GetDimension();
        }
        else if (objectType == ObjectType::CorrelationMatrix || objectType == ObjectType::CopulaCorrelation)
        {
            std::shared_ptr<BaseCorrelation> correlationMatrix = correlations[id];

            if (property_ == "variables_count") return correlationMatrix->GetDimension();
        }
        else if (objectType == ObjectType::Settings)
        {
            std::shared_ptr<Settings> settings = settingsValues[id];

            if (property_ == "max_parallel_processes") return settings->RunSettings->MaxParallelProcesses;
            else if (property_ == "minimum_samples") return settings->MinimumSamples;
            else if (property_ == "maximum_samples") return settings->MaximumSamples;
            else if (property_ == "maximum_samples_no_result") return settings->MaximumSamplesNoResult;
            else if (property_ == "minimum_iterations") return settings->MinimumIterations;
            else if (property_ == "maximum_iterations") return settings->MaximumIterations;
            else if (property_ == "minimum_directions") return settings->MinimumDirections;
            else if (property_ == "maximum_directions") return settings->MaximumDirections;
            else if (property_ == "minimum_variance_loops") return settings->MinimumVarianceLoops;
            else if (property_ == "maximum_variance_loops") return settings->MaximumVarianceLoops;
            else if (property_ == "minimum_failed_samples") return settings->MinimumFailedSamples;
            else if (property_ == "random_seed") return settings->RandomSettings->Seed;
            else if (property_ == "max_chunk_size") return settings->RunSettings->MaxChunkSize;
            else if (property_ == "max_messages") return settings->RunSettings->MaxMessages;
            else if (property_ == "relaxation_loops") return settings->RelaxationLoops;
            else if (property_ == "max_steps_sphere_search") return settings->StartPointSettings->maxStepsSphereSearch;
            else if (property_ == "max_clusters") return settings->MaxClusters;
        }
        else if (objectType == ObjectType::SensitivitySettings)
        {
            std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

            if (property_ == "max_parallel_processes") return settings->RunSettings->MaxParallelProcesses;
            else if (property_ == "max_chunk_size") return settings->RunSettings->MaxChunkSize;
            else if (property_ == "iterations") return settings->Iterations;
        }
        else if (objectType == ObjectType::UncertaintySettings)
        {
            std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

            if (property_ == "max_parallel_processes") return settings->RunSettings->MaxParallelProcesses;
            else if (property_ == "max_chunk_size") return settings->RunSettings->MaxChunkSize;
            else if (property_ == "minimum_samples") return settings->MinimumSamples;
            else if (property_ == "maximum_samples") return settings->MaximumSamples;
            else if (property_ == "maximum_iterations") return settings->MaximumIterations;
            else if (property_ == "minimum_directions") return settings->MinimumDirections;
            else if (property_ == "maximum_directions") return settings->MaximumDirections;
            else if (property_ == "random_seed") return settings->RandomSettings->Seed;
            else if (property_ == "required_samples")
                return Uncertainty::CrudeMonteCarloSettingsS::getRequiredSamples(settings->ProbabilityForConvergence, settings->VariationCoefficient);
            else if (property_ == "quantiles_count") return static_cast<int>(settings->RequestedQuantiles.size());
        }
        else if (objectType == ObjectType::UncertaintyResult)
        {
            std::shared_ptr<Uncertainty::UncertaintyResult> result = uncertaintyResults[id];

            if (property_ == "evaluations_count") return static_cast<int>(result->evaluations.size());
            else if (property_ == "quantile_evaluations_count") return static_cast<int>(result->quantileEvaluations.size());
            else if (property_ == "messages_count") return static_cast<int>(result->messages.size());
        }
        else if (objectType == ObjectType::StochastSettings)
        {
            std::shared_ptr<StochastSettings> stochastSettings = stochastSettingsValues[id];

            if (property_ == "intervals") return stochastSettings->Intervals;
        }
        else if (objectType == ObjectType::LengthEffectProject)
        {
            std::shared_ptr<LengthEffectProject> project = lengthEffectProjects[id];

            if (property_ == "correlation_lengths_count") return static_cast<int>(project->correlationLengths.size());
        }

        return 0;
    }

    int ProjectHandler::GetIdValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.GetIdValue(id, property_);
        }

        int newId = this->GetNewId();

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "validate") return admin.validationReportHandler.GetObjectId(std::make_shared<Logging::ValidationReport>(project->getValidationReport()));
        }
        else if (ProjectEntries::IsModelSettingsType(objectType))
        {
            std::shared_ptr<Models::ModelProjectSettings> settings = GetSettings(id);

            if (property_ == "validate") return admin.validationReportHandler.GetObjectId(std::make_shared<Logging::ValidationReport>(settings->getValidationReport()));
        }

        if (objectType == ObjectType::Project)
        {
            std::shared_ptr<ReliabilityProject> project = projects[id];

            if (property_ == "limit_state_function") return admin.limitStateFunctionHandler.GetObjectId(project->limitStateFunction);
            else if (property_ == "design_point") return admin.designPointHandler.GetObjectId(project->designPoint);
        }
        else if (objectType == ObjectType::RunProject)
        {
            std::shared_ptr<Models::RunProject> project = runProjects[id];

            if (property_ == "realization") return admin.evaluationHandler.GetObjectId(project->evaluation);
        }
        else if (objectType == ObjectType::UncertaintyProject)
        {
            std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

            if (property_ == "uncertainty_stochast") return admin.stochastHandler.GetObjectId(project->uncertaintyResult->stochast);
            else if (property_ == "uncertainty_result") return GetUncertaintyResultId(project->uncertaintyResult, newId);
            else if (property_ == "output_correlation_matrix") return GetCorrelationMatrixId(project->outputCorrelationMatrix, newId);
        }
        else if (objectType == ObjectType::SensitivityProject)
        {
            std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

            if (property_ == "result") return GetSensitivityResultId(project->sensitivityResult, newId);
        }
        else if (objectType == ObjectType::StochastSettings)
        {
            std::shared_ptr<StochastSettings> stochastSettings = stochastSettingsValues[id];

            if (property_ == "variable") return admin.stochastHandler.GetObjectId(stochastSettings->stochast);
        }
        else if (objectType == ObjectType::UncertaintyResult)
        {
            std::shared_ptr<Uncertainty::UncertaintyResult> result = uncertaintyResults[id];

            if (property_ == "variable") return admin.stochastHandler.GetObjectId(result->stochast);
        }
        else if (objectType == ObjectType::SensitivityValue)
        {
            std::shared_ptr<Sensitivity::SensitivityValue> result = sensitivityValues[id];

            if (property_ == "variable") return admin.stochastHandler.GetObjectId(result->stochast);
        }
        else if (objectType == ObjectType::FragilityCurveProject)
        {
            std::shared_ptr<FragilityCurveProject> project = fragilityCurveProjects[id];

            if (property_ == "design_point") return admin.designPointHandler.GetObjectId(project->designPoint);
            else if (property_ == "integrand") return admin.stochastHandler.GetObjectId(project->integrand);
            else if (property_ == "fragility_curve") admin.fragilityCurveHandler.GetObjectId(project->fragilityCurve);
            else if (property_ == "fragility_curve_normalized") admin.fragilityCurveHandler.GetObjectId(project->fragilityCurveNormalized);
        }
        else if (objectType == ObjectType::CombineProject)
        {
            std::shared_ptr<CombineProject> combineProject = combineProjects[id];

            if (property_ == "design_point") return admin.designPointHandler.GetObjectId(combineProject->designPoint);
            else if (property_ == "design_point_correlation_matrix") return GetCorrelationMatrixId(combineProject->correlationMatrix, newId);
            else if (property_ == "correlation_matrix") return GetSelfCorrelationMatrixId(combineProject->selfCorrelationMatrix, newId);
            else if (property_ == "validate") return admin.validationReportHandler.GetObjectId(std::make_shared<Logging::ValidationReport>(combineProject->getValidationReport()));
        }
        else if (objectType == ObjectType::ExcludingCombineProject)
        {
            std::shared_ptr<ExcludingCombineProject> combineProject = excludingCombineProjects[id];

            if (property_ == "design_point") return admin.designPointHandler.GetObjectId(combineProject->designPoint);
            else if (property_ == "validate") return admin.validationReportHandler.GetObjectId(std::make_shared<Logging::ValidationReport>(combineProject->getValidationReport()));
        }
        else if (objectType == ObjectType::LengthEffectProject)
        {
            std::shared_ptr<LengthEffectProject> project = lengthEffectProjects[id];

            if (property_ == "design_point") return admin.designPointHandler.GetObjectId(project->designPoint);
        }

        return 0;
    }

    void ProjectHandler::SetIntValue(int id, const std::string& property_, int value)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.SetIntValue(id, property_, value);
        }

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "settings") project->setSettings(GetSettings(value));
            else if (property_ == "correlation_matrix") project->correlation = correlations[value];
            else if (property_ == "copula_correlation") project->correlation = correlations[value];
            else if (property_ == "share_project") project->shareStochasts(GetProject(value));
            else if (property_ == "total_model_runs") project->modelRuns = value;
        }

        if (objectType == ObjectType::FragilityCurveProject)
        {
            std::shared_ptr<FragilityCurveProject> project = fragilityCurveProjects[id];

            if (property_ == "integrand") project->integrand = admin.stochastHandler.GetObject(value);
            else if (property_ == "fragility_curve") project->fragilityCurve = admin.fragilityCurveHandler.GetObject(value);
            else if (property_ == "fragility_curve_normalized") project->fragilityCurveNormalized = admin.fragilityCurveHandler.GetObject(value);
            else if (property_ == "settings") project->settings = fragilityCurveSettings[value];
        }
        else if (objectType == ObjectType::Settings)
        {
            std::shared_ptr<Settings> settings = settingsValues[id];

            if (property_ == "max_parallel_processes") settings->RunSettings->MaxParallelProcesses = value;
            else if (property_ == "max_chunk_size") settings->RunSettings->MaxChunkSize = value;
            else if (property_ == "minimum_samples") settings->MinimumSamples = value;
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
            else if (property_ == "max_chunk_size") settings->RunSettings->MaxChunkSize = value;
            else if (property_ == "max_messages") settings->RunSettings->MaxMessages = value;
            else if (property_ == "max_steps_sphere_search") settings->StartPointSettings->maxStepsSphereSearch = value;
            else if (property_ == "start_point")
            {
                std::shared_ptr<DesignPoint> designPoint = admin.designPointHandler.GetObject(value);
                settings->StochastSet->setStartPoint(designPoint->getSample());
            }
        }
        else if (objectType == ObjectType::SensitivitySettings)
        {
            std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

            if (property_ == "max_parallel_processes") settings->RunSettings->MaxParallelProcesses = value;
            else if (property_ == "max_chunk_size") settings->RunSettings->MaxChunkSize = value;
            else if (property_ == "iterations") settings->Iterations = value;
        }
        else if (objectType == ObjectType::UncertaintySettings)
        {
            std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

            if (property_ == "max_parallel_processes") settings->RunSettings->MaxParallelProcesses = value;
            else if (property_ == "max_chunk_size") settings->RunSettings->MaxChunkSize = value;
            else if (property_ == "minimum_samples") settings->MinimumSamples = value;
            else if (property_ == "maximum_samples") settings->MaximumSamples = value;
            else if (property_ == "maximum_iterations") settings->MaximumIterations = value;
            else if (property_ == "minimum_directions") settings->MinimumDirections = value;
            else if (property_ == "maximum_directions") settings->MaximumDirections = value;
            else if (property_ == "random_seed") settings->RandomSettings->Seed = value;
        }
        else if (objectType == ObjectType::StochastSettings)
        {
            std::shared_ptr<StochastSettings> stochastSettings = stochastSettingsValues[id];

            if (property_ == "variable") stochastSettings->stochast = value > 0 ? admin.stochastHandler.GetObject(value) : nullptr;
            else if (property_ == "intervals") stochastSettings->Intervals = value;
        }
        else if (objectType == ObjectType::Project)
        {
            std::shared_ptr<ReliabilityProject> reliabilityProject = projects[id];

            if (property_ == "limit_state_function") reliabilityProject->limitStateFunction = GetLimitStateFunction(value);
        }
        else if (objectType == ObjectType::CombineProject)
        {
            std::shared_ptr<CombineProject> combineProject = combineProjects[id];

            if (property_ == "settings") combineProject->settings = combineSettingsValues[value];
            else if (property_ == "correlation_matrix") combineProject->selfCorrelationMatrix = selfCorrelationMatrices[value];
            else if (property_ == "design_point_correlation_matrix") combineProject->correlationMatrix = correlations[value];
        }
        else if (objectType == ObjectType::ExcludingCombineProject)
        {
            std::shared_ptr<ExcludingCombineProject> combineProject = excludingCombineProjects[id];

            if (property_ == "settings") combineProject->settings = excludingCombineSettings[value];
        }
        else if (objectType == ObjectType::LengthEffectProject)
        {
            std::shared_ptr<LengthEffectProject> project = lengthEffectProjects[id];

            if (property_ == "correlation_matrix") project->selfCorrelationMatrix = selfCorrelationMatrices[value];
            else if (property_ == "design_point_cross_section") project->designPointCrossSection = admin.designPointHandler.GetObject(value);
        }
        else if (objectType == ObjectType::SelfCorrelationMatrix)
        {
            if (property_ == "correlation_stochast") tempIntValue = value;
        }
    }

    double ProjectHandler::GetIntArgValue(int id1, int id2, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id1);

        if (IsSupported(objectType))
        {
            return admin.GetIntArgValue(id1, id2, property_);
        }

        if (objectType == ObjectType::SelfCorrelationMatrix)
        {
            std::shared_ptr<SelfCorrelationMatrix> correlationMatrix = selfCorrelationMatrices[id1];
            std::shared_ptr<Stochast> stochast = admin.stochastHandler.GetObject(id2);

            if (property_ == "rho") return correlationMatrix->getSelfCorrelation(stochast);
        }
        return std::nan("");
    }

    void ProjectHandler::SetIntArgValue(int id1, int id2, const std::string& property_, double value)
    {
        ObjectType objectType = admin.GetObjectType(id1);

        if (objectType == ObjectType::SelfCorrelationMatrix)
        {
            std::shared_ptr<SelfCorrelationMatrix> correlationMatrix = selfCorrelationMatrices[id1];
            std::shared_ptr<Stochast> stochast = admin.stochastHandler.GetObject(id2);

            if (property_ == "rho") correlationMatrix->setSelfCorrelation(stochast, value);
        }
    }

    bool ProjectHandler::GetBoolValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.GetBoolValue(id, property_);
        }

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "is_valid") return project->isValid();
        }
        else if (ProjectEntries::IsModelSettingsType(objectType))
        {
            std::shared_ptr<Models::ModelProjectSettings> settings = GetSettings(id);

            if (property_ == "is_valid") return settings->isValid();
            else if (property_ == "save_realizations") return settings->RunSettings->SaveEvaluations;
            else if (property_ == "save_convergence") return settings->RunSettings->SaveConvergence;
            else if (property_ == "save_messages") return settings->RunSettings->SaveMessages;
            else if (property_ == "reuse_calculations") return settings->RunSettings->ReuseCalculations;
            else if (property_ == "allow_repository") return settings->RunSettings->AllowRepository;
            else if (property_ == "use_z_from_sample") return settings->RunSettings->UseZFromSample;
        }

        if (objectType == ObjectType::CorrelationMatrix)
        {
            std::shared_ptr<CorrelationMatrix> matrix = std::dynamic_pointer_cast<CorrelationMatrix>(correlations[id]);

            if (property_ == "is_identity") return matrix->IsIdentity();
            else if (property_ == "has_conflicting_correlations") return matrix->HasConflictingCorrelations();
            else if (property_ == "is_valid") return matrix->IsValid();
        }
        else if (objectType == ObjectType::StochastSettings)
        {
            std::shared_ptr<StochastSettings> stochastSettings = stochastSettingsValues[id];

            if (property_ == "is_initialization_allowed") return stochastSettings->IsInitializationAllowed;
            else if (property_ == "is_variance_allowed") return stochastSettings->IsVarianceAllowed;
        }
        else if (objectType == ObjectType::UncertaintySettings)
        {
            std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

            if (property_ == "derive_samples_from_variation_coefficient") return settings->DeriveSamplesFromVariationCoefficient;
            else if (property_ == "calculate_correlations") return settings->CalculateCorrelations;
            else if (property_ == "calculate_input_correlations") return settings->CalculateInputCorrelations;
            else if (property_ == "is_repeatable_random") return settings->RandomSettings->IsRepeatableRandom;
        }
        else if (objectType == ObjectType::Settings)
        {
            std::shared_ptr<Settings> setting = settingsValues[id];

            if (property_ == "all_quadrants") return setting->StartPointSettings->allQuadrants;
            else if (property_ == "is_repeatable_random") return setting->RandomSettings->IsRepeatableRandom;
            else if (property_ == "filter_at_non_convergence") return setting->FilterAtNonConvergence;
            else if (property_ == "clustering") return setting->Clustering;
            else if (property_ == "optimize_number_clusters") return setting->OptimizeNumberOfClusters;
            else if (property_ == "auto_maximum_samples") return setting->AutoMaximumSamples;
            else if (property_ == "start_point_on_limit_state") return setting->StartPointOnLimitState;
        }
        else if (objectType == ObjectType::CombineProject)
        {
            std::shared_ptr<CombineProject> project = combineProjects[id];

            if (property_ == "is_valid") return project->is_valid();
        }
        else if (objectType == ObjectType::ExcludingCombineProject)
        {
            std::shared_ptr<ExcludingCombineProject> project = excludingCombineProjects[id];

            if (property_ == "is_valid") return project->is_valid();
        }

        return false;
    }

    void ProjectHandler::SetBoolValue(int id, const std::string& property_, bool value)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.SetBoolValue(id, property_, value);
        }

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "callback_assigned") if (project->model != nullptr) project->model->callbackAssigned = value;
        }
        else if (ProjectEntries::IsModelSettingsType(objectType))
        {
            std::shared_ptr<Models::ModelProjectSettings> settings = GetSettings(id);

            if (property_ == "save_realizations") settings->RunSettings->SaveEvaluations = value;
            else if (property_ == "save_convergence") settings->RunSettings->SaveConvergence = value;
            else if (property_ == "save_messages") settings->RunSettings->SaveMessages = value;
            else if (property_ == "reuse_calculations") settings->RunSettings->ReuseCalculations = value;
            else if (property_ == "allow_repository") settings->RunSettings->AllowRepository = value;
            else if (property_ == "use_z_from_sample") settings->RunSettings->UseZFromSample = value;
            else if (property_ == "use_openmp_in_reliability") settings->RunSettings->UseOpenMPinReliability = value;
        }

        if (objectType == ObjectType::StochastSettings)
        {
            std::shared_ptr<StochastSettings> stochastSettings = stochastSettingsValues[id];

            if (property_ == "is_initialization_allowed") stochastSettings->IsInitializationAllowed = value;
            else if (property_ == "is_variance_allowed") stochastSettings->IsVarianceAllowed = value;
        }
        else if (objectType == ObjectType::UncertaintySettings)
        {
            std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

            if (property_ == "derive_samples_from_variation_coefficient") settings->DeriveSamplesFromVariationCoefficient = value;
            else if (property_ == "calculate_correlations") settings->CalculateCorrelations = value;
            else if (property_ == "calculate_input_correlations") settings->CalculateInputCorrelations = value;
            else if (property_ == "is_repeatable_random") settings->RandomSettings->IsRepeatableRandom = value;
        }
        else if (objectType == ObjectType::Settings)
        {
            std::shared_ptr<Settings> setting = settingsValues[id];

            if (property_ == "all_quadrants") setting->StartPointSettings->allQuadrants = value;
            else if (property_ == "is_repeatable_random") setting->RandomSettings->IsRepeatableRandom = value;
            else if (property_ == "filter_at_non_convergence") setting->FilterAtNonConvergence = value;
            else if (property_ == "clustering") setting->Clustering = value;
            else if (property_ == "optimize_number_clusters") setting->OptimizeNumberOfClusters = value;
            else if (property_ == "auto_maximum_samples") setting->AutoMaximumSamples = value;
            else if (property_ == "start_point_on_limit_state") setting->StartPointOnLimitState = value;
        }
    }

    std::string ProjectHandler::GetStringValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.GetStringValue(id, property_);
        }

        if (objectType == ObjectType::Settings)
        {
            std::shared_ptr<Settings> settings = settingsValues[id];

            if (property_ == "reliability_method") return Settings::getReliabilityMethodTypeString(settings->ReliabilityMethod);
            else if (property_ == "handle_invalid_type") return Models::RunSettings::getHandleInvalidTypeString(settings->RunSettings->handleInvalidType);
            else if (property_ == "reliability_result") return Settings::getReliabilityResultTypeString(settings->ReliabilityResult);
            else if (property_ == "model_return_type") return Models::RunSettings::getModelReturnTypeString(settings->RunSettings->modelReturnType);
            else if (property_ == "design_point_method") return DesignPointBuilder::getDesignPointMethodString(settings->designPointMethod);
            else if (property_ == "fragility_curve_design_point_method") return DesignPointBuilder::getDesignPointMethodString(settings->fragilityCurveDesignPointMethod);
            else if (property_ == "sample_method") return SubsetSimulationSettings::getSampleMethodString(settings->sampleMethod);
            else if (property_ == "start_method") return StartPointCalculatorSettings::getStartPointMethodString(settings->StartPointSettings->StartMethod);
            else if (property_ == "gradient_type") return Models::GradientSettings::getGradientTypeString(settings->GradientSettings->gradientType);
            else if (property_ == "model_varying_type") return DirectionReliabilitySettings::getModelVaryingTypeString(settings->DirectionSettings->modelVaryingType);
            else if (property_ == "lowest_message_type") return Logging::Message::getMessageTypeString(settings->RunSettings->LowestMessageType);
        }
        else if (objectType == ObjectType::FragilityCurveSettings)
        {
            std::shared_ptr<FragilityCurveIntegrationSettings> settings = fragilityCurveSettings[id];

            if (property_ == "design_point_method") return DesignPointBuilder::getDesignPointMethodString(settings->designPointMethod);
        }
        else if (objectType == ObjectType::RunProjectSettings)
        {
            std::shared_ptr<Models::RunProjectSettings> settings = runProjectSettings[id];

            if (property_ == "run_values_type") return Models::RunProjectSettings::getRunValuesTypeString(settings->runValuesType);
            else if (property_ == "lowest_message_type") return Logging::Message::getMessageTypeString(settings->RunSettings->LowestMessageType);
        }
        else if (objectType == ObjectType::UncertaintySettings)
        {
            std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

            if (property_ == "uncertainty_method") return Uncertainty::SettingsS::getUncertaintyMethodTypeString(settings->UncertaintyMethod);
            else if (property_ == "gradient_type") return Models::GradientSettings::getGradientTypeString(settings->GradientSettings->gradientType);
            else if (property_ == "lowest_message_type") return Logging::Message::getMessageTypeString(settings->RunSettings->LowestMessageType);
        }
        else if (objectType == ObjectType::UncertaintyProject)
        {
            std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

            if (property_ == "parameter") return project->parameter;
        }
        else if (objectType == ObjectType::SensitivityProject)
        {
            std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

            if (property_ == "parameter") return project->parameter;
        }
        else if (objectType == ObjectType::SensitivitySettings)
        {
            std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

            if (property_ == "sensitivity_method") return Sensitivity::SensitivitySettings::getSensitivityMethodTypeString(settings->SensitivityMethod);
            else if (property_ == "lowest_message_type") return Logging::Message::getMessageTypeString(settings->RunSettings->LowestMessageType);
        }
        else if (objectType == ObjectType::CombineSettings)
        {
            std::shared_ptr<CombineSettings> settings = combineSettingsValues[id];

            if (property_ == "combine_method") return DesignPointCombiner::getCombinerMethodString(settings->combinerMethod);
            else if (property_ == "combine_type") return DesignPointCombiner::getCombineTypeString(settings->combineType);
        }
        else if (objectType == ObjectType::ExcludingCombineSettings)
        {
            std::shared_ptr<ExcludingCombineSettings> settings = excludingCombineSettings[id];

            if (property_ == "combiner_method") return DesignPointCombiner::getExcludingCombinerMethodString(settings->combinerMethod);
        }
        else if (objectType == ObjectType::UncertaintyResult)
        {
            std::shared_ptr<Uncertainty::UncertaintyResult> result = uncertaintyResults[id];

            if (property_ == "identifier") return result->getIdentifier();
        }
        else if (objectType == ObjectType::SensitivityResult)
        {
            std::shared_ptr<Sensitivity::SensitivityResult> result = sensitivityResults[id];

            if (property_ == "identifier") return result->identifier;
        }

        return "";
    }

    void ProjectHandler::SetStringValue(int id, const std::string& property_, const std::string& value)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.SetStringValue(id, property_, value);
        }

        if (objectType == ObjectType::UncertaintyProject)
        {
            std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

            if (property_ == "parameter") project->parameter = value;
        }
        else if (objectType == ObjectType::SensitivityProject)
        {
            std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

            if (property_ == "parameter") project->parameter = value;
        }
        else if (objectType == ObjectType::Settings)
        {
            std::shared_ptr<Settings> settings = settingsValues[id];

            if (property_ == "reliability_method") settings->ReliabilityMethod = Settings::getReliabilityMethodType(value);
            else if (property_ == "reliability_result") settings->ReliabilityResult = Settings::getReliabilityResultType(value);
            else if (property_ == "handle_invalid_type") settings->RunSettings->handleInvalidType = Models::RunSettings::getHandleInvalidType(value);
            else if (property_ == "model_return_type") settings->RunSettings->modelReturnType = Models::RunSettings::getModelReturnType(value);
            else if (property_ == "design_point_method") settings->designPointMethod = DesignPointBuilder::getDesignPointMethod(value);
            else if (property_ == "fragility_curve_design_point_method") settings->fragilityCurveDesignPointMethod = DesignPointBuilder::getDesignPointMethod(value);
            else if (property_ == "sample_method") settings->sampleMethod = SubsetSimulationSettings::getSampleMethod(value);
            else if (property_ == "start_method") settings->StartPointSettings->StartMethod = StartPointCalculatorSettings::getStartPointMethod(value);
            else if (property_ == "gradient_type") settings->GradientSettings->gradientType = Models::GradientSettings::getGradientType(value);
            else if (property_ == "model_varying_type") settings->DirectionSettings->modelVaryingType = DirectionReliabilitySettings::getModelVaryingType(value);
            else if (property_ == "lowest_message_type") settings->RunSettings->LowestMessageType = Logging::Message::getMessageType(value);
        }
        else if (objectType == ObjectType::FragilityCurveSettings)
        {
            std::shared_ptr<FragilityCurveIntegrationSettings> settings = fragilityCurveSettings[id];

            if (property_ == "design_point_method") settings->designPointMethod = DesignPointBuilder::getDesignPointMethod(value);
        }
        else if (objectType == ObjectType::RunProjectSettings)
        {
            std::shared_ptr<Models::RunProjectSettings> settings = runProjectSettings[id];

            if (property_ == "run_values_type") settings->runValuesType = Models::RunProjectSettings::getRunValuesType(value);
            else if (property_ == "lowest_message_type") settings->RunSettings->LowestMessageType = Logging::Message::getMessageType(value);
        }
        else if (objectType == ObjectType::UncertaintySettings)
        {
            std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

            if (property_ == "uncertainty_method") settings->UncertaintyMethod = Uncertainty::SettingsS::getUncertaintyMethodType(value);
            else if (property_ == "gradient_type") settings->GradientSettings->gradientType = Models::GradientSettings::getGradientType(value);
            else if (property_ == "lowest_message_type") settings->RunSettings->LowestMessageType = Logging::Message::getMessageType(value);
        }
        else if (objectType == ObjectType::SensitivitySettings)
        {
            std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

            if (property_ == "sensitivity_method") settings->SensitivityMethod = Sensitivity::SensitivitySettings::getSensitivityMethodType(value);
            else if (property_ == "lowest_message_type") settings->RunSettings->LowestMessageType = Logging::Message::getMessageType(value);
        }
        else if (objectType == ObjectType::CombineSettings)
        {
            std::shared_ptr<CombineSettings> settings = combineSettingsValues[id];

            if (property_ == "combine_method") settings->combinerMethod = DesignPointCombiner::getCombinerMethod(value);
            else if (property_ == "combine_type") settings->combineType = DesignPointCombiner::getCombineType(value);
        }
        else if (objectType == ObjectType::ExcludingCombineSettings)
        {
            std::shared_ptr<ExcludingCombineSettings> settings = excludingCombineSettings[id];

            if (property_ == "combiner_method") settings->combinerMethod = DesignPointCombiner::getExcludingCombinerMethod(value);
        }
        else if (objectType == ObjectType::SensitivityResult)
        {
            std::shared_ptr<Sensitivity::SensitivityResult> sensitivityResult = sensitivityResults[id];

            if (property_ == "identifier") sensitivityResult->identifier = value;
        }
        else if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "model_name") project->model->name = value;
        }
    }

    void ProjectHandler::SetArrayValue(int id, const std::string& property_, double* values, int size)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            admin.SetArrayValue(id, property_, values, size);
            return;
        }

        if (objectType == ObjectType::LengthEffectProject)
        {
            std::shared_ptr<LengthEffectProject> lengthEffect = lengthEffectProjects[id];
            if (property_ == "correlation_lengths")
            {
                lengthEffect->correlationLengths.clear();
                for (size_t i = 0; i < size; i++)
                {
                    lengthEffect->correlationLengths.push_back(values[i]);
                }
            }
        }
    }

    std::vector<int> ProjectHandler::GetArrayIntValue(int id, const std::string& property_)
    {
        return std::vector<int>(0);
    }

    void ProjectHandler::SetArrayIntValue(int id, const std::string& property_, int* values, int size)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            admin.SetArrayIntValue(id, property_, values, size);
            return;
        }

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "variables")
            {
                project->stochasts.clear();
                for (int i = 0; i < size; i++)
                {
                    project->stochasts.push_back(admin.stochastHandler.GetObject(values[i]));
                }
            }
            else if (property_ == "input_parameters")
            {
                project->model->inputParameters.clear();
                for (int i = 0; i < size; i++)
                {
                    project->model->inputParameters.push_back(admin.modelParameterHandler.GetObject(values[i]));
                }
                project->updateStochasts();
            }
            else if (property_ == "output_parameters")
            {
                project->model->outputParameters.clear();
                for (int i = 0; i < size; i++)
                {
                    project->model->outputParameters.push_back(admin.modelParameterHandler.GetObject(values[i]));
                }
            }
        }

        if (objectType == ObjectType::CorrelationMatrix || objectType == ObjectType::CopulaCorrelation)
        {
            std::shared_ptr<BaseCorrelation> correlationMatrix = correlations[id];

            if (property_ == "variables")
            {
                std::vector<std::shared_ptr<Stochast>> correlationMatrixStochasts;
                for (int i = 0; i < size; i++)
                {
                    correlationMatrixStochasts.push_back(admin.stochastHandler.GetObject(values[i]));
                }

                correlationMatrix->Init(correlationMatrixStochasts);
            }
        }
        else if (objectType == ObjectType::UncertaintyProject)
        {
            std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

            if (property_ == "uncertainty_parameters")
            {
                project->uncertaintyParameters.clear();

                for (int i = 0; i < size; i++)
                {
                    project->uncertaintyParameters.push_back(admin.modelParameterHandler.GetObject(values[i]));
                }
            }
        }
        else if (objectType == ObjectType::SensitivityProject)
        {
            std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

            if (property_ == "sensitivity_parameters")
            {
                project->sensitivityParameters.clear();

                for (int i = 0; i < size; i++)
                {
                    project->sensitivityParameters.push_back(admin.modelParameterHandler.GetObject(values[i]));
                }
            }
        }
        else if (objectType == ObjectType::Settings)
        {
            std::shared_ptr<Settings> settings = settingsValues[id];

            if (property_ == "stochast_settings")
            {
                settings->StochastSet->stochastSettings.clear();
                for (int i = 0; i < size; i++)
                {
                    settings->StochastSet->stochastSettings.push_back(stochastSettingsValues[values[i]]);
                }
            }
        }
        else if (objectType == ObjectType::UncertaintySettings)
        {
            std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

            if (property_ == "stochast_settings")
            {
                settings->StochastSet->stochastSettings.clear();
                for (int i = 0; i < size; i++)
                {
                    settings->StochastSet->stochastSettings.push_back(stochastSettingsValues[values[i]]);
                }
            }
            else if (property_ == "quantiles")
            {
                settings->RequestedQuantiles.clear();
                for (int i = 0; i < size; i++)
                {
                    settings->RequestedQuantiles.push_back(admin.probabilityValueHandler.GetObject(values[i]));
                }
            }
        }
        else if (objectType == ObjectType::CombineProject)
        {
            std::shared_ptr<CombineProject> project = combineProjects[id];

            if (property_ == "design_points")
            {
                project->designPoints.clear();

                for (int i = 0; i < size; i++)
                {
                    project->designPoints.push_back(admin.designPointHandler.GetObject(values[i]));
                }
            }
        }
        else if (objectType == ObjectType::ExcludingCombineProject)
        {
            std::shared_ptr<ExcludingCombineProject> project = excludingCombineProjects[id];

            if (property_ == "design_points")
            {
                project->designPoints.clear();

                for (int i = 0; i < size; i++)
                {
                    project->designPoints.push_back(admin.designPointHandler.GetObject(values[i]));
                }
            }
            else if (property_ == "scenarios")
            {
                project->scenarios.clear();

                for (int i = 0; i < size; i++)
                {
                    project->scenarios.push_back(admin.scenarioHandler.GetObject(values[i]));
                }
            }
        }
    }

    double ProjectHandler::GetArgValue(int id, const std::string& property_, double argument)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.GetArgValue(id, property_, argument);
        }

        return std::nan("");
    }

    void ProjectHandler::SetArgValue(int id, const std::string& property_, double argument, double value)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            admin.SetArgValue(id, property_, argument, value);
        }
    }

    double ProjectHandler::GetIndexedValue(int id, const std::string& property_, int index)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.GetIndexedValue(id, property_, index);
        }

        if (ProjectEntries::IsStochast(objectType))
        {
        }
        else if (objectType == ObjectType::LengthEffectProject)
        {
            std::shared_ptr<LengthEffectProject> lengthEffect = lengthEffectProjects[id];
            if (property_ == "correlation_lengths")
            {
                return lengthEffect->correlationLengths[index];
            }
        }

        return std::nan("");
    }

    void ProjectHandler::SetIndexedValue(int id, const std::string& property_, int index, double value)
    {
        // not needed yet
    }

    double ProjectHandler::GetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (objectType == ObjectType::CorrelationMatrix || objectType == ObjectType::CopulaCorrelation)
        {
            std::shared_ptr<BaseCorrelation> correlationMatrix = correlations[id];

            if (property_ == "correlation") return correlationMatrix->GetCorrelation(admin.stochastHandler.GetObject(index1), admin.stochastHandler.GetObject(index2)).value;
            else if (property_ == "correlation_index") return correlationMatrix->GetCorrelation(index1, index2).value;
        }
        else if (objectType == ObjectType::SelfCorrelationMatrix)
        {
            std::shared_ptr<SelfCorrelationMatrix> selfCorrelationMatrix = selfCorrelationMatrices[id];

            if (property_ == "correlation")
            {
                int stochastId = tempIntValue;
                tempIntValue = 0;
                return selfCorrelationMatrix->getSelfCorrelation(admin.stochastHandler.GetObject(stochastId), admin.designPointHandler.GetObject(index1), admin.designPointHandler.GetObject(index2));
            }
        }

        return std::nan("");
    }

    void ProjectHandler::SetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2, double value)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (objectType == ObjectType::CorrelationMatrix)
        {
            std::shared_ptr<BaseCorrelation> correlationMatrix = correlations[id];

            if (property_ == "correlation") correlationMatrix->SetCorrelation(admin.stochastHandler.GetObject(index1), admin.stochastHandler.GetObject(index2), value, CorrelationType::Gaussian);
            else if (property_ == "correlation_index") correlationMatrix->SetCorrelation(index1, index2, value, CorrelationType::Gaussian);
        }
        else if (objectType == ObjectType::CopulaCorrelation)
        {
            std::shared_ptr<BaseCorrelation> correlationMatrix = correlations[id];

            if (property_ == "correlation")
            {
                CorrelationType type = static_cast<CorrelationType>(tempIntValue);
                correlationMatrix->SetCorrelation(admin.stochastHandler.GetObject(index1), admin.stochastHandler.GetObject(index2), value, type);
            }
        }
        else if (objectType == ObjectType::SelfCorrelationMatrix)
        {
            std::shared_ptr<SelfCorrelationMatrix> selfCorrelationMatrix = selfCorrelationMatrices[id];

            if (property_ == "correlation")
            {
                int stochastId = tempIntValue;
                tempIntValue = 0;
                selfCorrelationMatrix->setSelfCorrelation(admin.stochastHandler.GetObject(stochastId), admin.designPointHandler.GetObject(index1), admin.designPointHandler.GetObject(index2), value);
            }
        }
    }

    void ProjectHandler::SetIndexedIndexedIntValue(int id, const std::string& property_, int index1, int index2, int value)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (objectType == ObjectType::CopulaCorrelation)
        {
            std::shared_ptr<BaseCorrelation> correlationMatrix = correlations[id];

            if (property_ == "correlation")
            {
                tempIntValue = value;
            }
        }
    }

    int ProjectHandler::GetIndexedIntValue(int id, const std::string& property_, int index)
    {
        return 0;
    }

    int ProjectHandler::GetIndexedIdValue(int id, const std::string& property_, int index)
    {
        ObjectType objectType = admin.GetObjectType(id);
        int newId = this->GetNewId();

        if (IsSupported(objectType))
        {
            return admin.GetIndexedIdValue(id, property_, index);
        }

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "stochasts") return admin.stochastHandler.GetObjectId(project->stochasts[index]);
        }

        if (objectType == ObjectType::CorrelationMatrix || objectType == ObjectType::CopulaCorrelation)
        {
            std::shared_ptr<BaseCorrelation> correlationMatrix = correlations[id];

            if (property_ == "variables") return admin.stochastHandler.GetObjectId(correlationMatrix->GetStochast(index));
        }
        else if (objectType == ObjectType::UncertaintyProject)
        {
            std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

            if (property_ == "uncertainty_stochasts") return admin.stochastHandler.GetObjectId(project->uncertaintyResults[index]->stochast);
            else if (property_ == "uncertainty_results") return GetUncertaintyResultId(project->uncertaintyResults[index], newId);
            else if (property_ == "uncertainty_parameters") return admin.modelParameterHandler.GetObjectId(project->uncertaintyParameters[index]);
        }
        else if (objectType == ObjectType::SensitivityProject)
        {
            std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

            if (property_ == "results") return GetSensitivityResultId(project->sensitivityResults[index], newId);
            else if (property_ == "sensitivity_parameters") return admin.modelParameterHandler.GetObjectId(project->sensitivityParameters[index]);
        }
        else if (objectType == ObjectType::SensitivityResult)
        {
            std::shared_ptr<Sensitivity::SensitivityResult> result = sensitivityResults[id];

            if (property_ == "values") return GetSensitivityValueId(result->values[index], newId);
            else if (property_ == "evaluations") return admin.evaluationHandler.GetObjectId(result->evaluations[index]);
            else if (property_ == "messages") return admin.messageHandler.GetObjectId(result->messages[index]);
        }
        else if (objectType == ObjectType::UncertaintySettings)
        {
            std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

            if (property_ == "quantiles") return admin.probabilityValueHandler.GetObjectId(settings->RequestedQuantiles[index]);
        }
        else if (objectType == ObjectType::UncertaintyResult)
        {
            std::shared_ptr<Uncertainty::UncertaintyResult> result = uncertaintyResults[id];

            if (property_ == "evaluations") return admin.evaluationHandler.GetObjectId(result->evaluations[index]);
            else if (property_ == "quantile_evaluations") return admin.evaluationHandler.GetObjectId(result->quantileEvaluations[index]);
            else if (property_ == "messages") return admin.messageHandler.GetObjectId(result->messages[index]);
        }

        return 0;
    }

    void ProjectHandler::SetCallBack(int id, const std::string& property_, Models::ZValuesCallBack callBack)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "model") project->model = std::make_shared<Models::ZModel>(callBack);
        }
    }

    void ProjectHandler::SetMultipleCallBack(int id, const std::string& property_, Models::ZValuesMultipleCallBack callBack)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "model")
            {
                if (project->model == nullptr)
                {
                    project->model = std::make_shared<Models::ZModel>();
                }

                project->model->setMultipleCallback(callBack);
            }
        }
    }

    void ProjectHandler::SetEmptyCallBack(int id, const std::string& property_, Models::EmptyCallBack callBack)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "run_samples")
            {
                if (project->model == nullptr)
                {
                    project->model = std::make_shared<Models::ZModel>();
                }

                project->model->setRunMethod(callBack);
            }
            else if (property_ == "next")
            {
                if (project->model == nullptr)
                {
                    project->model = std::make_shared<Models::ZModel>();
                }

                project->model->setNextCalculation(callBack);
            }
        }
    }

    void ProjectHandler::SetProgressCallBacks(int id, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            project->progressIndicator = std::make_shared<Models::ProgressIndicator>(progress, detailed, textual);
        }
        else if (objectType == ObjectType::CombineProject)
        {
            std::shared_ptr<CombineProject> project = combineProjects[id];

            project->progressIndicator = std::make_shared<Models::ProgressIndicator>(progress, detailed, textual);
        }
    }

    void ProjectHandler::SetModelSampleCallBack(int id, const std::string& property_, Models::ModelSampleCallback callBack)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "model")
            {
                if (project->model == nullptr)
                {
                    project->model = std::make_shared<Models::ZModel>();
                }

                project->model->setModelSampleCallback(callBack);
            }
        }
    }

    void ProjectHandler::SetMultipleModelSampleCallBack(int id, const std::string& property_, Models::MultipleModelSampleCallback callBack)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "model")
            {
                if (project->model == nullptr)
                {
                    project->model = std::make_shared<Models::ZModel>();
                }

                project->model->setMultipleModelSampleCallback(callBack);
            }
        }
    }

    void ProjectHandler::Execute(int id, const std::string& method_)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (ProjectEntries::IsStochast(objectType))
        {
        }
        else if (objectType == ObjectType::CorrelationMatrix)
        {
            std::shared_ptr<CorrelationMatrix> matrix = std::dynamic_pointer_cast<CorrelationMatrix>(correlations[id]);

            if (method_ == "resolve_conflicting_correlations") matrix->resolveConflictingCorrelations();
        }
        else if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (method_ == "run") project->run();
            else if (method_ == "stop") project->stop();
        }
        else if (objectType == ObjectType::FragilityCurveProject)
        {
            std::shared_ptr<FragilityCurveProject> project = fragilityCurveProjects[id];

            if (method_ == "run") project->run();
        }
        else if (objectType == ObjectType::CombineProject)
        {
            std::shared_ptr<CombineProject> project = combineProjects[id];

            if (method_ == "run") project->run();
        }
        else if (objectType == ObjectType::ExcludingCombineProject)
        {
            std::shared_ptr<ExcludingCombineProject> project = excludingCombineProjects[id];

            if (method_ == "run") project->run();
        }
        else if (objectType == ObjectType::LengthEffectProject)
        {
            std::shared_ptr<LengthEffectProject> project = lengthEffectProjects[id];

            if (method_ == "run") project->run();
        }
    }

    int ProjectHandler::GetCorrelationMatrixId(const std::shared_ptr<BaseCorrelation>& correlationMatrix, int newId)
    {
        if (correlationMatrix == nullptr)
        {
            return 0;
        }
        else
        {
            if (!correlationIds.contains(correlationMatrix))
            {
                std::lock_guard lock(mtx);

                correlations[newId] = correlationMatrix;
                admin.RegisterType(newId, ObjectType::CorrelationMatrix);
                correlationIds[correlationMatrix] = newId;
            }

            return correlationIds[correlationMatrix];
        }
    }

    int ProjectHandler::GetSelfCorrelationMatrixId(const std::shared_ptr<SelfCorrelationMatrix>& correlationMatrix, int newId)
    {
        if (correlationMatrix == nullptr)
        {
            return 0;
        }
        else
        {
            if (!selfCorrelationIds.contains(correlationMatrix))
            {
                std::lock_guard lock(mtx);

                selfCorrelationMatrices[newId] = correlationMatrix;
                admin.RegisterType(newId, ObjectType::SelfCorrelationMatrix);
                selfCorrelationIds[correlationMatrix] = newId;
            }

            return selfCorrelationIds[correlationMatrix];
        }
    }

    int ProjectHandler::GetStatus(const std::string& command) const
    {
        if (command == "count_entries")
        {
            return admin.GetSize();
        }
        return -1;
    }

    int ProjectHandler::GetUncertaintyResultId(const std::shared_ptr<Uncertainty::UncertaintyResult>& result, int newId)
    {
        if (result == nullptr)
        {
            return 0;
        }
        else
        {
            if (!uncertaintyResultsIds.contains(result))
            {
                std::lock_guard lock(mtx);

                uncertaintyResults[newId] = result;
                admin.RegisterType(newId, ObjectType::UncertaintyResult);
                uncertaintyResultsIds[result] = newId;
            }

            return uncertaintyResultsIds[result];
        }
    }

    int ProjectHandler::GetSensitivityResultId(const std::shared_ptr<Sensitivity::SensitivityResult>& result, int newId)
    {
        if (result == nullptr)
        {
            return 0;
        }
        else
        {
            if (!sensitivityResultsIds.contains(result))
            {
                std::lock_guard lock(mtx);

                sensitivityResults[newId] = result;
                admin.RegisterType(newId, ObjectType::SensitivityResult);
                sensitivityResultsIds[result] = newId;
            }

            return sensitivityResultsIds[result];
        }
    }

    int ProjectHandler::GetSensitivityValueId(const std::shared_ptr<Sensitivity::SensitivityValue>& result, int newId)
    {
        if (result == nullptr)
        {
            return 0;
        }
        else
        {
            if (!sensitivityValuesIds.contains(result))
            {
                std::lock_guard lock(mtx);

                sensitivityValues[newId] = result;
                admin.RegisterType(newId, ObjectType::SensitivityValue);
                sensitivityValuesIds[result] = newId;
            }

            return sensitivityValuesIds[result];
        }
    }

    std::shared_ptr<DesignPointIds> ProjectHandler::GetDesignPointIds(int id)
    {
        return nullptr;
    }

    std::shared_ptr<Models::ModelProject> ProjectHandler::GetProject(int id)
    {
        if (projects.contains(id))
        {
            return std::static_pointer_cast<Models::ModelProject>(projects[id]);
        }
        else if (runProjects.contains(id))
        {
            return std::static_pointer_cast<Models::ModelProject>(runProjects[id]);
        }
        else if (uncertaintyProjects.contains(id))
        {
            return std::static_pointer_cast<Models::ModelProject>(uncertaintyProjects[id]);
        }
        else if (sensitivityProjects.contains(id))
        {
            return std::static_pointer_cast<Models::ModelProject>(sensitivityProjects[id]);
        }
        else
        {
            return nullptr;
        }
    }

    std::shared_ptr<Models::ModelProjectSettings> ProjectHandler::GetSettings(int id)
    {
        if (settingsValues.contains(id))
        {
            return settingsValues[id];
        }
        else if (runProjectSettings.contains(id))
        {
            return runProjectSettings[id];
        }
        else if (uncertaintySettingsValues.contains(id))
        {
            return uncertaintySettingsValues[id];
        }
        else if (sensitivitySettingsValues.contains(id))
        {
            return sensitivitySettingsValues[id];
        }
        else
        {
            return nullptr;
        }
    }

    std::shared_ptr<LimitStateFunction> ProjectHandler::GetLimitStateFunction(int id)
    {
        if (admin.limitStateFunctionHandler.Contains(id))
        {
            return admin.limitStateFunctionHandler.GetObject(id);
        }
        else if (admin.combinedLimitStateFunctionHandler.Contains(id))
        {
            return admin.combinedLimitStateFunctionHandler.GetObject(id);
        }
        else if (admin.probabilityLimitStateFunctionHandler.Contains(id))
        {
            return admin.probabilityLimitStateFunctionHandler.GetObject(id);
        }
        else
        {
            return nullptr;
        }
    }

}

