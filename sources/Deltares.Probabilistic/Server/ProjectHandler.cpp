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
        case ObjectType::FragilityCurveProject:
            fragilityCurveProjects[id] = std::make_shared<FragilityCurveProject>();
            break;
        case ObjectType::CombineProject:
            combineProjects[id] = std::make_shared<CombineProject>();
            break;
        case ObjectType::ExcludingCombineProject:
            excludingCombineProjects[id] = std::make_shared<ExcludingCombineProject>();
            break;
        case ObjectType::RunProject:
            runProjects[id] = std::make_shared<Models::RunProject>();
            break;
        case ObjectType::UncertaintyProject:
            uncertaintyProjects[id] = std::make_shared<Uncertainty::UncertaintyProject>();
            break;
        case ObjectType::SensitivityProject:
            sensitivityProjects[id] = std::make_shared<Sensitivity::SensitivityProject>();
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
        case ObjectType::FragilityCurveProject: fragilityCurveProjects.erase(id); break;
        case ObjectType::CombineProject: combineProjects.erase(id); break;
        case ObjectType::ExcludingCombineProject: excludingCombineProjects.erase(id); break;
        case ObjectType::RunProject: runProjects.erase(id); break;
        case ObjectType::UncertaintyProject: uncertaintyProjects.erase(id); break;
        case ObjectType::SensitivityProject: sensitivityProjects.erase(id); break;
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

        if (objectType == ObjectType::LengthEffectProject)
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

        if (objectType == ObjectType::LengthEffectProject)
        {
            std::shared_ptr<LengthEffectProject> length_effect = lengthEffectProjects[id];

            if (property_ == "length") length_effect->length = value;
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

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "validate") return admin.validationReportHandler.GetObjectId(std::make_shared<Logging::ValidationReport>(project->getValidationReport()));
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
            else if (property_ == "uncertainty_result") return admin.uncertaintyResultHandler.GetObjectId(project->uncertaintyResult);
            else if (property_ == "output_correlation_matrix") return admin.correlationMatrixHandler.GetObjectId(project->outputCorrelationMatrix);
        }
        else if (objectType == ObjectType::SensitivityProject)
        {
            std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

            if (property_ == "result") return admin.sensitivityResultHandler.GetObjectId(project->sensitivityResult);
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
            else if (property_ == "design_point_correlation_matrix")
            {
                if (std::dynamic_pointer_cast<CopulaCorrelation>(combineProject->correlationMatrix) != nullptr)
                {
                    return admin.copulaCorrelationHandler.GetObjectId(std::dynamic_pointer_cast<CopulaCorrelation>(combineProject->correlationMatrix));
                }
                else
                {
                    return admin.correlationMatrixHandler.GetObjectId(std::dynamic_pointer_cast<CorrelationMatrix>(combineProject->correlationMatrix));
                }
            }
            else if (property_ == "correlation_matrix") return admin.selfCorrelationMatrixHandler.GetObjectId(combineProject->selfCorrelationMatrix);
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

            if (property_ == "correlation_matrix") project->correlation = admin.correlationMatrixHandler.GetObject(value);
            else if (property_ == "copula_correlation") project->correlation = admin.copulaCorrelationHandler.GetObject(value);
            else if (property_ == "share_project") project->shareStochasts(GetProject(value));
            else if (property_ == "total_model_runs") project->modelRuns = value;
        }

        if (objectType == ObjectType::FragilityCurveProject)
        {
            std::shared_ptr<FragilityCurveProject> project = fragilityCurveProjects[id];

            if (property_ == "integrand") project->integrand = admin.stochastHandler.GetObject(value);
            else if (property_ == "fragility_curve") project->fragilityCurve = admin.fragilityCurveHandler.GetObject(value);
            else if (property_ == "fragility_curve_normalized") project->fragilityCurveNormalized = admin.fragilityCurveHandler.GetObject(value);
            else if (property_ == "settings") project->settings = admin.fragilityCurveSettingsHandler.GetObject(value);
        }
        else if (objectType == ObjectType::Project)
        {
            std::shared_ptr<ReliabilityProject> reliabilityProject = projects[id];

            if (property_ == "settings") reliabilityProject->setSettings(admin.reliabilitySettingsHandler.GetObject(value));
            else if (property_ == "limit_state_function") reliabilityProject->limitStateFunction = GetLimitStateFunction(value);
        }
        else if (objectType == ObjectType::UncertaintyProject)
        {
            std::shared_ptr<Uncertainty::UncertaintyProject> uncertaintyProject = uncertaintyProjects[id];

            if (property_ == "settings") uncertaintyProject->setSettings(admin.uncertaintySettingsHandler.GetObject(value));
        }
        else if (objectType == ObjectType::SensitivityProject)
        {
            std::shared_ptr<Sensitivity::SensitivityProject> sensitivityProject = sensitivityProjects[id];

            if (property_ == "settings") sensitivityProject->setSettings(admin.sensitivitySettingsHandler.GetObject(value));
        }
        else if (objectType == ObjectType::RunProject)
        {
            std::shared_ptr<Models::RunProject> runProject = runProjects[id];

            if (property_ == "settings") runProject->setSettings(admin.runProjectSettingsHandler.GetObject(value));
        }
        else if (objectType == ObjectType::CombineProject)
        {
            std::shared_ptr<CombineProject> combineProject = combineProjects[id];

            if (property_ == "settings") combineProject->settings = admin.combineSettingsHandler.GetObject(value);
            else if (property_ == "correlation_matrix") combineProject->selfCorrelationMatrix = admin.selfCorrelationMatrixHandler.GetObject(value);
            else if (property_ == "design_point_correlation_matrix")
            {
                if (admin.copulaCorrelationHandler.Contains(value))
                {
                    combineProject->correlationMatrix = admin.copulaCorrelationHandler.GetObject(value);
                }
                else
                {
                    combineProject->correlationMatrix = admin.correlationMatrixHandler.GetObject(value);
                }
            }
        }
        else if (objectType == ObjectType::ExcludingCombineProject)
        {
            std::shared_ptr<ExcludingCombineProject> combineProject = excludingCombineProjects[id];

            if (property_ == "settings") combineProject->settings = admin.excludingCombineSettingsHandler.GetObject(value);
        }
        else if (objectType == ObjectType::LengthEffectProject)
        {
            std::shared_ptr<LengthEffectProject> project = lengthEffectProjects[id];

            if (property_ == "correlation_matrix") project->selfCorrelationMatrix = admin.selfCorrelationMatrixHandler.GetObject(value);
            else if (property_ == "design_point_cross_section") project->designPointCrossSection = admin.designPointHandler.GetObject(value);
        }
    }

    double ProjectHandler::GetIntArgValue(int id1, int id2, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id1);

        if (IsSupported(objectType))
        {
            return admin.GetIntArgValue(id1, id2, property_);
        }

        return std::nan("");
    }

    void ProjectHandler::SetIntArgValue(int id1, int id2, const std::string& property_, double value)
    {
        ObjectType objectType = admin.GetObjectType(id1);

        if (IsSupported(objectType))
        {
            return admin.SetIntArgValue(id1, id2, property_, value);
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

        if (objectType == ObjectType::CombineProject)
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
    }

    std::string ProjectHandler::GetStringValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.GetStringValue(id, property_);
        }

        if (objectType == ObjectType::UncertaintyProject)
        {
            std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

            if (property_ == "parameter") return project->parameter;
        }
        else if (objectType == ObjectType::SensitivityProject)
        {
            std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

            if (property_ == "parameter") return project->parameter;
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

        if (objectType == ObjectType::UncertaintyProject)
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

        if (objectType == ObjectType::LengthEffectProject)
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

        if (IsSupported(objectType))
        {
            return admin.GetIndexedIndexedValue(id, property_, index1, index2);
        }

        return std::nan("");
    }

    void ProjectHandler::SetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2, double value)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            admin.SetIndexedIndexedValue(id, property_, index1, index2, value);
        }
    }

    void ProjectHandler::SetIndexedIndexedIntValue(int id, const std::string& property_, int index1, int index2, int value)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.SetIndexedIndexedIntValue(id, property_, index1, index2, value);
        }
    }

    int ProjectHandler::GetIndexedIntValue(int id, const std::string& property_, int index)
    {
        return 0;
    }

    int ProjectHandler::GetIndexedIdValue(int id, const std::string& property_, int index)
    {
        ObjectType objectType = admin.GetObjectType(id);

        if (IsSupported(objectType))
        {
            return admin.GetIndexedIdValue(id, property_, index);
        }

        if (ProjectEntries::IsModelProjectType(objectType))
        {
            std::shared_ptr<Models::ModelProject> project = GetProject(id);

            if (property_ == "stochasts") return admin.stochastHandler.GetObjectId(project->stochasts[index]);
        }

        if (objectType == ObjectType::UncertaintyProject)
        {
            std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

            if (property_ == "uncertainty_stochasts") return admin.stochastHandler.GetObjectId(project->uncertaintyResults[index]->stochast);
            else if (property_ == "uncertainty_results") return admin.uncertaintyResultHandler.GetObjectId(project->uncertaintyResults[index]);
            else if (property_ == "uncertainty_parameters") return admin.modelParameterHandler.GetObjectId(project->uncertaintyParameters[index]);
        }
        else if (objectType == ObjectType::SensitivityProject)
        {
            std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

            if (property_ == "results") return admin.sensitivityResultHandler.GetObjectId(project->sensitivityResults[index]);
            else if (property_ == "sensitivity_parameters") return admin.modelParameterHandler.GetObjectId(project->sensitivityParameters[index]);
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

        if (IsSupported(objectType))
        {
            admin.Execute(id, method_);
            return;
        }

        if (ProjectEntries::IsModelProjectType(objectType))
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

    int ProjectHandler::GetStatus(const std::string& command) const
    {
        if (command == "count_entries")
        {
            return admin.GetSize();
        }
        return -1;
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

