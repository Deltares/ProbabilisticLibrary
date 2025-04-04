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
#include "../Reliability/ReliabilityProject.h"
#include "../Reliability/FragilityCurve.h"
#include "../Reliability/FragilityCurveProject.h"
#include "../Reliability/LimitStateFunction.h"
#include "../Reliability/Settings.h"
#include "../Model/RunProject.h"
#include "../Model/RunProjectSettings.h"
#include "../Sensitivity/SensitivityProject.h"
#include "../Sensitivity/SettingsS.h"
#include "../Combine/CombineProject.h"
#include "../Combine/CombineSettings.h"
#include "../Combine/ExcludingCombineProject.h"
#include "../Combine/ExcludingCombineSettings.h"
#include "../Combine/LengthEffectProject.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/DiscreteValue.h"

namespace Deltares
{
    namespace Server
    {
        class ProjectHandler : public BaseHandler
        {
        public:
            bool CanHandle(std::string object_type) override;
            void Create(std::string object_type, int id) override;
            void Destroy(int id) override;
            bool ShouldClose() override;
            double GetValue(int id, const std::string property_) override;
            void SetValue(int id, const std::string property_, double value) override;
            int GetIntValue(int id, std::string property_) override;
            void SetIntValue(int id, std::string property_, int value) override;
            int GetIdValue(int id, std::string property_, int newId) override;
            double GetIntArgValue(int id1, int id2, std::string property_) override;
            void SetIntArgValue(int id1, int id2, std::string property_, double value) override;
            bool GetBoolValue(int id, std::string property_) override;
            void SetBoolValue(int id, std::string property_, bool value) override;
            std::string GetStringValue(int id, std::string property_) override;
            void SetStringValue(int id, std::string property_, std::string value) override;
            void SetArrayValue(int id, std::string property_, double* values, int size) override;
            std::vector<int> GetArrayIntValue(int id, std::string property_) override;
            void SetArrayIntValue(int id, std::string property_, int* values, int size) override;
            double GetArgValue(int id, std::string property_, double argument) override;
            void SetArgValue(int id, std::string property_, double argument, double value) override;
            double GetIndexedValue(int id, std::string property_, int index) override;
            void SetIndexedValue(int id, std::string property_, int index, double value) override;
            double GetIndexedIndexedValue(int id, std::string property_, int index1, int index2) override;
            void SetIndexedIndexedValue(int id, std::string property_, int index1, int index2, double value) override;
            int GetIndexedIntValue(int id, std::string property_, int index) override;
            int GetIndexedIdValue(int id, std::string property_, int index, int newId) override;
            void SetCallBack(int id, std::string property_, Models::ZValuesCallBack callBack) override;
            void SetMultipleCallBack(int id, std::string property_, Models::ZValuesMultipleCallBack callBack) override;
            void SetEmptyCallBack(int id, std::string property_, Models::EmptyCallBack callBack) override;
            void Execute(int id, std::string method_) override;

            int GetProbabilityValueId(std::shared_ptr<Statistics::ProbabilityValue> probability, int newId);
            int GetStochastId(std::shared_ptr<Statistics::Stochast> stochast, int newId);
            int GetFragilityCurveId(std::shared_ptr<Reliability::FragilityCurve> fragilityCurve, int newId);
            int GetCorrelationMatrixId(std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, int newid);
            int GetLimitStateFunctionId(std::shared_ptr<Reliability::LimitStateFunction> limitStateFunction, int newid);
            int GetDesignPointId(std::shared_ptr<Reliability::DesignPoint> designPoint, int newId);
            int GetAlphaId(std::shared_ptr<Models::StochastPointAlpha> alpha, int newId);
            int GetSensitivityResultId(std::shared_ptr<Sensitivity::SensitivityResult> result, int newId);
            int GetHistogramValueId(std::shared_ptr<Statistics::HistogramValue> histogramValue, int newId);
            int GetDiscreteValueId(std::shared_ptr<Statistics::DiscreteValue> discreteValue, int newId);
            int GetFragilityValueId(std::shared_ptr<Statistics::FragilityValue> fragilityValue, int newId);
            int GetContributingStochastId(std::shared_ptr<Statistics::ContributingStochast> contributingStochast, int newId);
            int GetConditionalValueId(std::shared_ptr<Statistics::VariableStochastValue> conditionalValue, int newId);
            int GetEvaluationId(std::shared_ptr<Deltares::Reliability::Evaluation> evaluation, int newId);
            int GetMessageId(std::shared_ptr<Deltares::Models::Message> message, int newId);

            std::shared_ptr <Reliability::DesignPoint> GetDesignPoint(int id)
            {
                return designPoints[id];
            }
        protected:
            virtual std::shared_ptr<Reliability::DesignPointIds> GetDesignPointIds(int id);
        private:
            enum ObjectType {StandardNormal, Message, ProbabilityValue, Project, ModelParameter, LimitStateFunction, Stochast, DiscreteValue, HistogramValue, FragilityValue,
                ContributingStochast, ConditionalValue, CorrelationMatrix, Scenario, Settings, StochastSettings, DesignPoint, Alpha, FragilityCurve, FragilityCurveProject, Evaluation,
                CombineProject, CombineSettings, ExcludingCombineProject, ExcludingCombineSettings, SelfCorrelationMatrix, SensitivityProject, SensitivitySettings, SensitivityResult,
                LengthEffectProject};
            ObjectType GetType(std::string object_type);
            std::unordered_map<int, Deltares::Server::ProjectHandler::ObjectType> types;

            std::unordered_map<int, std::shared_ptr<Statistics::Stochast>> stochasts;
            std::unordered_map<int, std::shared_ptr<Statistics::ProbabilityValue>> probabilityValues;
            std::unordered_map<int, std::shared_ptr<Models::Message>> messages;
            std::unordered_map<int, std::shared_ptr<Reliability::ReliabilityProject>> projects;
            std::unordered_map<int, std::shared_ptr<Models::ModelInputParameter>> modelParameters;
            std::unordered_map<int, std::shared_ptr<Reliability::LimitStateFunction>> limitStateFunctions;
            std::unordered_map<int, std::shared_ptr<Statistics::DiscreteValue>> discreteValues;
            std::unordered_map<int, std::shared_ptr<Statistics::HistogramValue>> histogramValues;
            std::unordered_map<int, std::shared_ptr<Statistics::FragilityValue>> fragilityValues;
            std::unordered_map<int, std::shared_ptr<Statistics::ContributingStochast>> contributingStochasts;
            std::unordered_map<int, std::shared_ptr<Statistics::VariableStochastValue>> conditionalValues;
            std::unordered_map<int, std::shared_ptr<Statistics::CorrelationMatrix>> correlationMatrices;
            std::unordered_map<int, std::shared_ptr<Statistics::Scenario>> scenarios;
            std::unordered_map<int, std::shared_ptr<Reliability::Settings>> settingsValues;
            std::unordered_map<int, std::shared_ptr<Reliability::StochastSettings>> stochastSettingsValues;
            std::unordered_map<int, std::shared_ptr<Reliability::DesignPoint>> designPoints;
            std::unordered_map<int, std::shared_ptr<Reliability::StochastPointAlpha>> alphas;
            std::unordered_map<int, std::shared_ptr<Reliability::FragilityCurve>> fragilityCurves;
            std::unordered_map<int, std::shared_ptr<Reliability::FragilityCurveProject>> fragilityCurveProjects;
            std::unordered_map<int, std::shared_ptr<Reliability::Evaluation>> evaluations;
            std::unordered_map<int, std::shared_ptr<Reliability::CombineProject>> combineProjects;
            std::unordered_map<int, std::shared_ptr<Reliability::CombineSettings>> combineSettingsValues;
            std::unordered_map<int, std::shared_ptr<Reliability::ExcludingCombineProject>> excludingCombineProjects;
            std::unordered_map<int, std::shared_ptr<Reliability::ExcludingCombineSettings>> excludingCombineSettings;
            std::unordered_map<int, std::shared_ptr<Reliability::LengthEffectProject>> lengthEffectProjects;
            std::unordered_map<int, std::shared_ptr<Statistics::SelfCorrelationMatrix>> selfCorrelationMatrices;
            std::unordered_map<int, std::shared_ptr<Models::RunProject>> runProjects;
            std::unordered_map<int, std::shared_ptr<Models::RunProjectSettings>> runProjectSettings;
            std::unordered_map<int, std::shared_ptr<Sensitivity::SensitivityProject>> sensitivityProjects;
            std::unordered_map<int, std::shared_ptr<Sensitivity::SettingsS>> sensitivitySettingsValues;
            std::unordered_map<int, std::shared_ptr<Sensitivity::SensitivityResult>> sensitivityResults;

            std::unordered_map<std::shared_ptr<Reliability::LimitStateFunction>, int> limitStateFunctionIds;
            std::unordered_map<std::shared_ptr<Reliability::Settings>, int> settingsValuesIds;
            std::unordered_map<std::shared_ptr<Reliability::DesignPoint>, int> designPointIds;
            std::unordered_map<std::shared_ptr<Sensitivity::SensitivityResult>, int> sensitivityResultsIds;
            std::unordered_map<std::shared_ptr<Reliability::StochastPointAlpha>, int> alphaIds;
            std::unordered_map<std::shared_ptr<Reliability::FragilityCurve>, int> fragilityCurveIds;
            std::unordered_map<std::shared_ptr<Statistics::Stochast>, int> stochastIds;
            std::unordered_map<std::shared_ptr<Statistics::ProbabilityValue>, int> probabilityValueIds;
            std::unordered_map<std::shared_ptr<Statistics::CorrelationMatrix>, int> correlationMatrixIds;
            std::unordered_map<std::shared_ptr<Statistics::HistogramValue>, int> histogramValueIds;
            std::unordered_map<std::shared_ptr<Statistics::DiscreteValue>, int> discreteValueIds;
            std::unordered_map<std::shared_ptr<Statistics::FragilityValue>, int> fragilityValueIds;
            std::unordered_map<std::shared_ptr<Statistics::ContributingStochast>, int> contributingStochastIds;
            std::unordered_map<std::shared_ptr<Statistics::VariableStochastValue>, int> conditionalValueIds;
            std::unordered_map<std::shared_ptr<Reliability::Evaluation>, int> evaluationIds;
            std::unordered_map<std::shared_ptr<Models::Message>, int> messageIds;

            std::unordered_map <std::string, std::vector<double>> tempValues;

            void UpdateValidationMessages(const std::vector<std::shared_ptr<Models::Message>>& newMessages);
            std::vector<std::shared_ptr<Models::Message>> validationMessages;
            std::shared_ptr<Models::ModelProject> GetProject(int id);
        };
    }
}
