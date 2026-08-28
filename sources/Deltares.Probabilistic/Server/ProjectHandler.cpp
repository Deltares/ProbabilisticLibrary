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

namespace Deltares::Server
{
    ProjectHandler::ProjectHandler()
    {
        InitializeHandlers();
    }

    bool ProjectHandler::CanHandle(const std::string& object_type)
    {
        return ProjectEntries::CanHandle(object_type);
    }

    int ProjectHandler::GetNewId()
    {
        return admin.GetNewId();
    }

    int ProjectHandler::Create(const std::string& objectTypeString)
    {
        ObjectType objectType = ProjectEntries::GetType(objectTypeString);
        return handlers[objectType]->Create();
    }

    void ProjectHandler::Destroy(int id)
    {
        if (admin.Contains(id))
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->Destroy(id);
        }
    }

    bool ProjectHandler::ShouldClose()
    {
        return admin.IsEmpty();
    }

    double ProjectHandler::GetValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetValue(id, property_);
    }

    void ProjectHandler::SetValue(int id, const std::string& property_, double value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetValue(id, property_, value);
    }

    std::string ProjectHandler::GetStringValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetStringValue(id, property_);
    }

    void ProjectHandler::SetStringValue(int id, const std::string& property_, const std::string& value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetStringValue(id, property_, value);
    }

    int ProjectHandler::GetIntValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetIntValue(id, property_);
    }

    int ProjectHandler::GetIdValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetIdValue(id, property_);
    }

    void ProjectHandler::SetIntValue(int id, const std::string& property_, int value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetIntValue(id, property_, value);
    }

    bool ProjectHandler::GetBoolValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetBoolValue(id, property_);
    }

    void ProjectHandler::SetBoolValue(int id, const std::string& property_, bool value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetBoolValue(id, property_, value);
    }

    double ProjectHandler::GetIntArgValue(int id1, int id2, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id1);
        return handlers[objectType]->GetIntArgValue(id1, id2, property_);
    }

    void ProjectHandler::SetIntArgValue(int id1, int id2, const std::string& property_, double value)
    {
        ObjectType objectType = admin.GetObjectType(id1);
        return handlers[objectType]->SetIntArgValue(id1, id2, property_, value);
    }

    int ProjectHandler::GetIndexedIntValue(int id, const std::string& property_, int index)
    {
        return 0;
    }

    int ProjectHandler::GetIndexedIdValue(int id, const std::string& property_, int index)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetIndexedIdValue(id, property_, index);
    }

    void ProjectHandler::SetArrayValue(int id, const std::string& property_, double* values, int size)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetArrayValue(id, property_, values, size);
    }

    std::vector<int> ProjectHandler::GetArrayIntValue(int id, const std::string& property_)
    {
        return std::vector<int>(0);
    }

    void ProjectHandler::SetArrayIntValue(int id, const std::string& property_, int* values, int size)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetArrayIntValue(id, property_, values, size);
    }

    double ProjectHandler::GetArgValue(int id, const std::string& property_, double argument)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetArgValue(id, property_, argument);
    }

    void ProjectHandler::SetArgValue(int id, const std::string& property_, double argument, double value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetArgValue(id, property_, argument, value);
    }

    double ProjectHandler::GetIndexedValue(int id, const std::string& property_, int index)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetIndexedValue(id, property_, index);
    }

    void ProjectHandler::SetIndexedValue(int id, const std::string& property_, int index, double value)
    {
        // not needed yet
    }

    double ProjectHandler::GetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetIndexedIndexedValue(id, property_, index1, index2);
    }

    void ProjectHandler::SetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2, double value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->SetIndexedIndexedValue(id, property_, index1, index2, value);
    }

    void ProjectHandler::SetIndexedIndexedIntValue(int id, const std::string& property_, int index1, int index2, int value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->SetIndexedIndexedIntValue(id, property_, index1, index2, value);
    }

    void ProjectHandler::Execute(int id, const std::string& method_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->Execute(id, method_);
    }

    void ProjectHandler::SetProgressCallBacks(int id, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetProgressCallBacks(id, progress, detailed, textual);
    }

    void ProjectHandler::SetCallBack(int id, const std::string& property_, Models::ZValuesCallBack callback)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetCallBack(id, property_, callback);
    }

    void ProjectHandler::SetMultipleCallBack(int id, const std::string& property_, Models::ZValuesMultipleCallBack callback)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetMultipleCallBack(id, property_, callback);
    }

    void ProjectHandler::SetEmptyCallBack(int id, const std::string& property_, Models::EmptyCallBack callback)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetEmptyCallBack(id, property_, callback);
    }

    void ProjectHandler::SetModelSampleCallBack(int id, const std::string& property_, Models::ModelSampleCallback callback)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetModelSampleCallBack(id, property_, callback);
    }

    void ProjectHandler::SetMultipleModelSampleCallBack(int id, const std::string& property_, Models::MultipleModelSampleCallback callback)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetMultipleModelSampleCallBack(id, property_, callback);
    }

    int ProjectHandler::GetStatus(const std::string& command) const
    {
        if (command == "count_entries")
        {
            return admin.GetSize();
        }
        return -1;
    }

    void ProjectHandler::InitializeHandlers()
    {
        validationReportHandler.messageHandler = &messageHandler;
        scenarioHandler.stochastHandler = &stochastHandler;

        stochastHandler.validationReportHandler = &validationReportHandler;
        stochastHandler.discreteValueHandler = &discreteValueHandler;
        stochastHandler.histogramValueHandler = &histogramValueHandler;
        stochastHandler.fragilityValueHandler = &fragilityValueHandler;
        stochastHandler.conditionalValueHandler = &conditionalValueHandler;
        stochastHandler.contributingStochastHandler = &contributingStochastHandler;

        contributingStochastHandler.stochastCallback = [this](const int id) {return this->stochastHandler.GetObject(id); };
        contributingStochastHandler.stochastIdCallback = [this](const std::shared_ptr<Statistics::Stochast>& stochast) {return this->stochastHandler.GetObjectId(stochast); };

        fragilityCurveHandler.stochastHandler = &stochastHandler;
        fragilityCurveHandler.designPointIdCallback = [this](const std::shared_ptr<Reliability::DesignPoint>& designPoint) {return this->designPointHandler.GetObjectId(designPoint); };

        fragilityValueHandler.designPointCallback = [this](const int id) {return this->designPointHandler.GetObject(id); };
        fragilityValueHandler.designPointIdCallback = [this](const std::shared_ptr<Reliability::DesignPoint>& designPoint) {return this->designPointHandler.GetObjectId(designPoint); };

        correlationMatrixHandler.stochastHandler = &stochastHandler;
        copulaCorrelationHandler.stochastHandler = &stochastHandler;
        selfCorrelationMatrixHandler.stochastHandler = &stochastHandler;

        alphaHandler.stochastHandler = &stochastHandler;
        alphaHandler.fragilityCurveHandler = &fragilityCurveHandler;

        designPointHandler.convergenceReportHandler = &convergenceReportHandler;
        designPointHandler.evaluationHandler = &evaluationHandler;
        designPointHandler.reliabilityResultHandler = &reliabilityResultHandler;
        designPointHandler.messageHandler = &messageHandler;
        designPointHandler.alphaHandler = &alphaHandler;
        designPointHandler.designPointIdsCallback = [this](const int id) {return this->GetDesignPointIds(id); };

        stochastPointHandler.alphaHandler = &alphaHandler;

        probabilityLimitStateFunctionHandler.fragilityCurveHandler = &fragilityCurveHandler;

        combinedLimitStateFunctionHandler.limitStateFunctionHandler = &limitStateFunctionHandler;

        uncertaintyResultHandler.stochastHandler = &stochastHandler;
        uncertaintyResultHandler.evaluationHandler = &evaluationHandler;
        uncertaintyResultHandler.messageHandler = &messageHandler;

        sensitivityResultHandler.sensitivityValueHandler = &sensitivityValueHandler;
        sensitivityResultHandler.evaluationHandler = &evaluationHandler;
        sensitivityResultHandler.messageHandler = &messageHandler;

        sensitivityValueHandler.stochastHandler = &stochastHandler;

        stochastSettingsHandler.stochastHandler = &stochastHandler;

        modelProjectSettingsHandler.validationReportHandler = &validationReportHandler;

        runProjectSettingsHandler.modelProjectSettingsHandler = &modelProjectSettingsHandler;
        sensitivitySettingsHandler.modelProjectSettingsHandler = &modelProjectSettingsHandler;
        uncertaintySettingsHandler.modelProjectSettingsHandler = &modelProjectSettingsHandler;
        uncertaintySettingsHandler.stochastSettingsHandler = &stochastSettingsHandler;
        uncertaintySettingsHandler.probabilityValueHandler = &probabilityValueHandler;
        reliabilitySettingsHandler.modelProjectSettingsHandler = &modelProjectSettingsHandler;
        reliabilitySettingsHandler.stochastSettingsHandler = &stochastSettingsHandler;
        reliabilitySettingsHandler.designPointCallback = [this](const int id) {return this->designPointHandler.GetObject(id); };

        combineProjectHandler.combineSettingsHandler = &combineSettingsHandler;
        combineProjectHandler.designPointHandler = &designPointHandler;
        combineProjectHandler.correlationMatrixHandler = &correlationMatrixHandler;
        combineProjectHandler.copulaCorrelationHandler = &copulaCorrelationHandler;
        combineProjectHandler.selfCorrelationMatrixHandler = &selfCorrelationMatrixHandler;
        combineProjectHandler.validationReportHandler = &validationReportHandler;

        excludingCombineProjectHandler.excludingCombineSettingsHandler = &excludingCombineSettingsHandler;
        excludingCombineProjectHandler.designPointHandler = &designPointHandler;
        excludingCombineProjectHandler.scenarioHandler = &scenarioHandler;
        excludingCombineProjectHandler.validationReportHandler = &validationReportHandler;

        lengthEffectProjectHandler.designPointHandler = &designPointHandler;
        lengthEffectProjectHandler.selfCorrelationMatrixHandler = &selfCorrelationMatrixHandler;

        fragilityCurveProjectHandler.designPointHandler = &designPointHandler;
        fragilityCurveProjectHandler.stochastHandler = &stochastHandler;
        fragilityCurveProjectHandler.fragilityCurveHandler = &fragilityCurveHandler;
        fragilityCurveProjectHandler.fragilityCurveSettingsHandler = &fragilityCurveSettingsHandler;

        modelProjectHandler.validationReportHandler = &validationReportHandler;
        modelProjectHandler.modelParameterHandler = &modelParameterHandler;
        modelProjectHandler.stochastHandler = &stochastHandler;
        modelProjectHandler.correlationMatrixHandler = &correlationMatrixHandler;
        modelProjectHandler.copulaCorrelationHandler = &copulaCorrelationHandler;
        modelProjectHandler.modelProjectCallback = [this](const int id) { return this->GetProject(id); };

        runProjectHandler.modelProjectHandler = &modelProjectHandler;
        runProjectHandler.runProjectSettingsHandler = &runProjectSettingsHandler;
        runProjectHandler.evaluationHandler = &evaluationHandler;

        sensitivityProjectHandler.modelProjectHandler = &modelProjectHandler;
        sensitivityProjectHandler.sensitivitySettingsHandler = &sensitivitySettingsHandler;
        sensitivityProjectHandler.sensitivityResultHandler = &sensitivityResultHandler;
        sensitivityProjectHandler.modelParameterHandler = &modelParameterHandler;

        uncertaintyProjectHandler.modelProjectHandler = &modelProjectHandler;
        uncertaintyProjectHandler.modelParameterHandler = &modelParameterHandler;
        uncertaintyProjectHandler.uncertaintySettingsHandler = &uncertaintySettingsHandler;
        uncertaintyProjectHandler.uncertaintyResultHandler = &uncertaintyResultHandler;
        uncertaintyProjectHandler.stochastHandler = &stochastHandler;
        uncertaintyProjectHandler.correlationMatrixHandler = &correlationMatrixHandler;

        reliabilityProjectHandler.modelProjectHandler = &modelProjectHandler;
        reliabilityProjectHandler.reliabilitySettingsHandler = &reliabilitySettingsHandler;
        reliabilityProjectHandler.limitStateFunctionHandler = &limitStateFunctionHandler;
        reliabilityProjectHandler.designPointHandler = &designPointHandler;
        reliabilityProjectHandler.limitStateFunctionCallback = [this](int id) { return this->GetLimitStateFunction(id); };

        handlers[ObjectType::HistogramValue] = &histogramValueHandler;
        handlers[ObjectType::DiscreteValue] = &discreteValueHandler;
        handlers[ObjectType::FragilityValue] = &fragilityValueHandler;
        handlers[ObjectType::Message] = &messageHandler;
        handlers[ObjectType::Evaluation] = &evaluationHandler;
        handlers[ObjectType::ProbabilityValue] = &probabilityValueHandler;
        handlers[ObjectType::StandardNormal] = &standardNormalHandler;
        handlers[ObjectType::Stochast] = &stochastHandler;
        handlers[ObjectType::Scenario] = &scenarioHandler;
        handlers[ObjectType::FragilityCurve] = &fragilityCurveHandler;
        handlers[ObjectType::ValidationReport] = &validationReportHandler;
        handlers[ObjectType::ModelParameter] = &modelParameterHandler;
        handlers[ObjectType::LimitStateFunction] = &limitStateFunctionHandler;
        handlers[ObjectType::ProbabilityLimitStateFunction] = &probabilityLimitStateFunctionHandler;
        handlers[ObjectType::CombinedLimitStateFunction] = &combinedLimitStateFunctionHandler;
        handlers[ObjectType::ReliabilityResult] = &reliabilityResultHandler;
        handlers[ObjectType::ConvergenceReport] = &convergenceReportHandler;
        handlers[ObjectType::ConditionalValue] = &conditionalValueHandler;
        handlers[ObjectType::ContributingStochast] = &contributingStochastHandler;
        handlers[ObjectType::CorrelationMatrix] = &correlationMatrixHandler;
        handlers[ObjectType::CopulaCorrelation] = &copulaCorrelationHandler;
        handlers[ObjectType::SelfCorrelationMatrix] = &selfCorrelationMatrixHandler;
        handlers[ObjectType::Alpha] = &alphaHandler;
        handlers[ObjectType::StochastPoint] = &stochastPointHandler;
        handlers[ObjectType::DesignPoint] = &designPointHandler;
        handlers[ObjectType::UncertaintyResult] = &uncertaintyResultHandler;
        handlers[ObjectType::SensitivityResult] = &sensitivityResultHandler;
        handlers[ObjectType::SensitivityValue] = &sensitivityValueHandler;
        handlers[ObjectType::RunProjectSettings] = &runProjectSettingsHandler;
        handlers[ObjectType::SensitivitySettings] = &sensitivitySettingsHandler;
        handlers[ObjectType::UncertaintySettings] = &uncertaintySettingsHandler;
        handlers[ObjectType::Settings] = &reliabilitySettingsHandler;
        handlers[ObjectType::FragilityCurveSettings] = &fragilityCurveSettingsHandler;
        handlers[ObjectType::CombineSettings] = &combineSettingsHandler;
        handlers[ObjectType::ExcludingCombineSettings] = &excludingCombineSettingsHandler;
        handlers[ObjectType::StochastSettings] = &stochastSettingsHandler;
        handlers[ObjectType::CombineProject] = &combineProjectHandler;
        handlers[ObjectType::ExcludingCombineProject] = &excludingCombineProjectHandler;
        handlers[ObjectType::LengthEffectProject] = &lengthEffectProjectHandler;
        handlers[ObjectType::FragilityCurveProject] = &fragilityCurveProjectHandler;
        handlers[ObjectType::RunProject] = &runProjectHandler;
        handlers[ObjectType::SensitivityProject] = &sensitivityProjectHandler;
        handlers[ObjectType::UncertaintyProject] = &uncertaintyProjectHandler;
        handlers[ObjectType::Project] = &reliabilityProjectHandler;

        for (const auto& [objectType, handler] : handlers)
        {
            handler->SetAdmin(&this->admin);
        }
    }

    std::shared_ptr<Models::ModelProject> ProjectHandler::GetProject(int id)
    {
        if (runProjectHandler.Contains(id))
        {
            return runProjectHandler.GetObject(id);
        }
        else if (sensitivityProjectHandler.Contains(id))
        {
            return sensitivityProjectHandler.GetObject(id);
        }
        else if (uncertaintyProjectHandler.Contains(id))
        {
            return uncertaintyProjectHandler.GetObject(id);
        }
        else if (reliabilityProjectHandler.Contains(id))
        {
            return reliabilityProjectHandler.GetObject(id);
        }
        else
        {
            return nullptr;
        }
    }

    std::shared_ptr<Reliability::LimitStateFunction> ProjectHandler::GetLimitStateFunction(int id)
    {
        if (limitStateFunctionHandler.Contains(id))
        {
            return limitStateFunctionHandler.GetObject(id);
        }
        else if (combinedLimitStateFunctionHandler.Contains(id))
        {
            return combinedLimitStateFunctionHandler.GetObject(id);
        }
        else if (probabilityLimitStateFunctionHandler.Contains(id))
        {
            return probabilityLimitStateFunctionHandler.GetObject(id);
        }
        else
        {
            return nullptr;
        }
    }

    std::shared_ptr<Reliability::DesignPointIds> ProjectHandler::GetDesignPointIds(int id)
    {
        return nullptr;
    }
}

