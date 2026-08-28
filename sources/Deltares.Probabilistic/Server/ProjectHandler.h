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

#include "BaseHandler.h"
#include "ProjectEntries.h"
#include "../Reliability/ReliabilityProject.h"
#include "../Reliability/LimitStateFunction.h"
#include "../Reliability/CombinedLimitStateFunction.h"
#include "../Reliability/Settings.h"
#include "../Model/RunProject.h"

#include "Handlers/CombineProjectHandler.h"
#include "Handlers/CombineSettingsHandler.h"
#include "Handlers/ExcludingCombineSettingsHandler.h"
#include "Handlers/ContributingStochastHandler.h"
#include "Handlers/ConvergenceReportHandler.h"
#include "Handlers/CopulaCorrelationHandler.h"
#include "Handlers/CorrelationMatrixHandler.h"
#include "Handlers/DesignPointHandler.h"
#include "Handlers/DiscreteValueHandler.h"
#include "Handlers/EvaluationHandler.h"
#include "Handlers/ExcludingCombineProjectHandler.h"
#include "Handlers/FragilityCurveHandler.h"
#include "Handlers/FragilityCurveProjectHandler.h"
#include "Handlers/FragilityCurveSettingsHandler.h"
#include "Handlers/FragilityValueHandler.h"
#include "Handlers/HistogramValueHandler.h"
#include "Handlers/LengthEffectProjectHandler.h"
#include "Handlers/LimitStateFunctionHandler.h"
#include "Handlers/CombinedLimitStateFunctionHandler.h"
#include "Handlers/MessageHandler.h"
#include "Handlers/ModelParameterHandler.h"
#include "Handlers/ModelProjectHandler.h"
#include "Handlers/ProbabilityLimitStateFunctionHandler.h"
#include "Handlers/ProbabilityValueHandler.h"
#include "Handlers/ReliabilityResultHandler.h"
#include "Handlers/ScenarioHandler.h"
#include "Handlers/SelfCorrelationMatrixHandler.h"
#include "Handlers/SensitivityResultHandler.h"
#include "Handlers/SensitivityValueHandler.h"
#include "Handlers/StandardNormalHandler.h"
#include "Handlers/StochastHandler.h"
#include "Handlers/StochastPointAlphaHandler.h"
#include "Handlers/StochastPointHandler.h"
#include "Handlers/StochastSettingsHandler.h"
#include "Handlers/UncertaintyResultHandler.h"
#include "Handlers/ValidationReportHandler.h"
#include "Handlers/ModelProjectSettingsHandler.h"
#include "Handlers/RunProjectSettingsHandler.h"
#include "Handlers/SensitivitySettingsHandler.h"
#include "Handlers/UncertaintySettingsHandler.h"
#include "Handlers/ReliabilitySettingsHandler.h"
#include "Handlers/RunProjectHandler.h"
#include "Handlers/ReliabilityProjectHandler.h"
#include "Handlers/SensitivityProjectHandler.h"
#include "Handlers/UncertaintyProjectHandler.h"

namespace Deltares::Server
{
    class ProjectHandler : public BaseHandler
    {
    public:
        ProjectHandler();

        bool CanHandle(const std::string& object_type) override;
        int GetNewId() override;
        int Create(const std::string& object_type) override;
        void Destroy(int id) override;
        bool ShouldClose() override;
        double GetValue(int id, const std::string& property_) override;
        void SetValue(int id, const std::string& property_, double value) override;
        int GetIntValue(int id, const std::string& property_) override;
        void SetIntValue(int id, const std::string& property_, int value) override;
        int GetIdValue(int id, const std::string& property_) override;
        double GetIntArgValue(int id1, int id2, const std::string& property_) override;
        void SetIntArgValue(int id1, int id2, const std::string& property_, double value) override;
        bool GetBoolValue(int id, const std::string& property_) override;
        void SetBoolValue(int id, const std::string& property_, bool value) override;
        std::string GetStringValue(int id, const std::string& property_) override;
        void SetStringValue(int id, const std::string& property_, const std::string& value) override;
        void SetArrayValue(int id, const std::string& property_, double* values, int size) override;
        std::vector<int> GetArrayIntValue(int id, const std::string& property_) override;
        void SetArrayIntValue(int id, const std::string& property_, int* values, int size) override;
        double GetArgValue(int id, const std::string& property_, double argument) override;
        void SetArgValue(int id, const std::string& property_, double argument, double value) override;
        double GetIndexedValue(int id, const std::string& property_, int index) override;
        void SetIndexedValue(int id, const std::string& property_, int index, double value) override;
        double GetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2) override;
        void SetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2, double value) override;
        void SetIndexedIndexedIntValue(int id, const std::string& property_, int index1, int index2, int value) override;
        int GetIndexedIntValue(int id, const std::string& property_, int index) override;
        int GetIndexedIdValue(int id, const std::string& property_, int index) override;
        void SetCallBack(int id, const std::string& property_, Models::ZValuesCallBack callBack) override;
        void SetMultipleCallBack(int id, const std::string& property_, Models::ZValuesMultipleCallBack callBack) override;
        void SetEmptyCallBack(int id, const std::string& property_, Models::EmptyCallBack callBack) override;
        void SetProgressCallBacks(int id, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual) override;
        void SetModelSampleCallBack(int id, const std::string& property_, Models::ModelSampleCallback callBack) override;
        void SetMultipleModelSampleCallBack(int id, const std::string& property_, Models::MultipleModelSampleCallback callBack) override;
        void Execute(int id, const std::string& method_) override;

        int GetStatus(const std::string& command) const;

    protected:
        virtual std::shared_ptr<Reliability::DesignPointIds> GetDesignPointIds(int id);
    private:

        ObjectHandlerAdmin admin;

        void InitializeHandlers();

        HistogramValueHandler histogramValueHandler;
        DiscreteValueHandler discreteValueHandler;
        FragilityValueHandler fragilityValueHandler;
        MessageHandler messageHandler;
        EvaluationHandler evaluationHandler;
        ProbabilityValueHandler probabilityValueHandler;
        StandardNormalHandler standardNormalHandler;
        StochastHandler stochastHandler;
        FragilityCurveHandler fragilityCurveHandler;
        ScenarioHandler scenarioHandler;
        ValidationReportHandler validationReportHandler;
        ModelParameterHandler modelParameterHandler;
        LimitStateFunctionHandler limitStateFunctionHandler;
        ProbabilityLimitStateFunctionHandler probabilityLimitStateFunctionHandler;
        CombinedLimitStateFunctionHandler combinedLimitStateFunctionHandler;
        ReliabilityResultHandler reliabilityResultHandler;
        ConvergenceReportHandler convergenceReportHandler;
        ContributingStochastHandler contributingStochastHandler;
        ConditionalValueHandler conditionalValueHandler;
        CorrelationMatrixHandler correlationMatrixHandler;
        CopulaCorrelationHandler copulaCorrelationHandler;
        SelfCorrelationMatrixHandler selfCorrelationMatrixHandler;
        StochastPointAlphaHandler alphaHandler;
        StochastPointHandler stochastPointHandler;
        DesignPointHandler designPointHandler;
        UncertaintyResultHandler uncertaintyResultHandler;
        SensitivityResultHandler sensitivityResultHandler;
        SensitivityValueHandler sensitivityValueHandler;
        StochastSettingsHandler stochastSettingsHandler;
        ModelProjectSettingsHandler modelProjectSettingsHandler;
        RunProjectSettingsHandler runProjectSettingsHandler;
        SensitivitySettingsHandler sensitivitySettingsHandler;
        UncertaintySettingsHandler uncertaintySettingsHandler;
        ReliabilitySettingsHandler reliabilitySettingsHandler;
        FragilityCurveSettingsHandler fragilityCurveSettingsHandler;
        CombineSettingsHandler combineSettingsHandler;
        ExcludingCombineSettingsHandler excludingCombineSettingsHandler;
        CombineProjectHandler combineProjectHandler;
        ExcludingCombineProjectHandler excludingCombineProjectHandler;
        LengthEffectProjectHandler lengthEffectProjectHandler;
        ModelProjectHandler modelProjectHandler;
        RunProjectHandler runProjectHandler;
        ReliabilityProjectHandler reliabilityProjectHandler;
        SensitivityProjectHandler sensitivityProjectHandler;
        UncertaintyProjectHandler uncertaintyProjectHandler;
        FragilityCurveProjectHandler fragilityCurveProjectHandler;

        std::unordered_map<ObjectType, ObjectHandler*> handlers;

        std::shared_ptr<Models::ModelProject> GetProject(int id);
        std::shared_ptr<Reliability::LimitStateFunction> GetLimitStateFunction(int id);
    };
}
