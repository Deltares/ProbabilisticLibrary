// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once
#include "BaseHandler.h"
#include "../Reliability/ReliabilityProject.h"
#include "../Reliability/Settings.h"
#include "../Sensitivity/SensitivityProject.h"
#include "../Sensitivity/SettingsS.h"
#include "../Combine/CombineProject.h"
#include "../Combine/CombineSettings.h"
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
            double GetValue(int id, const std::string property_) override;
            void SetValue(int id, const std::string property_, double value) override;
            int GetIntValue(int id, std::string property_) override;
            void SetIntValue(int id, std::string property_, int value) override;
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
            void SetCallBack(int id, std::string property_, Models::ZValuesCallBack callBack) override;
            void SetEmptyCallBack(int id, std::string property_, Models::ZEmptyCallBack callBack) override;
            void Execute(int id, std::string method_) override;

            int GetStochastId(std::shared_ptr<Statistics::Stochast> stochast);
            int GetCorrelationMatrixId(std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix);
            int GetDesignPointId(std::shared_ptr<Reliability::DesignPoint> designPoint);
            int GetAlphaId(std::shared_ptr<Models::StochastPointAlpha> alpha);
            int GetHistogramValueId(std::shared_ptr<Statistics::HistogramValue> histogramValue);
            int GetDiscreteValueId(std::shared_ptr<Statistics::DiscreteValue> discreteValue);
            int GetFragilityValueId(std::shared_ptr<Statistics::FragilityValue> fragilityValue);
            int GetContributingStochastId(std::shared_ptr<Statistics::ContributingStochast> contributingStochast);
            int GetMessageId(std::shared_ptr<Deltares::Models::Message> message);
        private:
            enum ObjectType {StandardNormal, Message, Project, ModelParameter, Stochast, DiscreteValue, HistogramValue, FragilityValue,
                ContributingStochast, CorrelationMatrix, Settings, StochastSettings, DesignPoint, Alpha,
                CombineProject, CombineSettings, SelfCorrelationMatrix, SensitivityProject, SensitivitySettings};

            int id_ = 0;
            std::unordered_map<int, Deltares::Server::ProjectHandler::ObjectType> types;

            std::unordered_map<int, std::shared_ptr<Statistics::Stochast>> stochasts;
            std::unordered_map<int, std::shared_ptr<Models::Message>> messages;
            std::unordered_map<int, std::shared_ptr<Reliability::ReliabilityProject>> projects;
            std::unordered_map<int, std::shared_ptr<Models::ModelParameter>> modelParameters;
            std::unordered_map<int, std::shared_ptr<Statistics::DiscreteValue>> discreteValues;
            std::unordered_map<int, std::shared_ptr<Statistics::HistogramValue>> histogramValues;
            std::unordered_map<int, std::shared_ptr<Statistics::FragilityValue>> fragilityValues;
            std::unordered_map<int, std::shared_ptr<Statistics::ContributingStochast>> contributingStochasts;
            std::unordered_map<int, std::shared_ptr<Statistics::CorrelationMatrix>> correlationMatrices;
            std::unordered_map<int, std::shared_ptr<Reliability::Settings>> settingsValues;
            std::unordered_map<int, std::shared_ptr<Reliability::StochastSettings>> stochastSettingsValues;
            std::unordered_map<int, std::shared_ptr<Reliability::DesignPoint>> designPoints;
            std::unordered_map<int, std::shared_ptr<Reliability::StochastPointAlpha>> alphas;
            std::unordered_map<int, std::shared_ptr<Reliability::CombineProject>> combineProjects;
            std::unordered_map<int, std::shared_ptr<Reliability::CombineSettings>> combineSettingsValues;
            std::unordered_map<int, std::shared_ptr<Statistics::SelfCorrelationMatrix>> selfCorrelationMatrices;
            std::unordered_map<int, std::shared_ptr<Sensitivity::SensitivityProject>> sensitivityProjects;
            std::unordered_map<int, std::shared_ptr<Sensitivity::SettingsS>> sensitivitySettingsValues;

            std::unordered_map<std::shared_ptr<Reliability::DesignPoint>, int> designPointIds;
            std::unordered_map<std::shared_ptr<Reliability::StochastPointAlpha>, int> alphaIds;
            std::unordered_map<std::shared_ptr<Statistics::Stochast>, int> stochastIds;
            std::unordered_map<std::shared_ptr<Statistics::CorrelationMatrix>, int> correlationMatrixIds;
            std::unordered_map<std::shared_ptr<Statistics::HistogramValue>, int> histogramValueIds;
            std::unordered_map<std::shared_ptr<Statistics::DiscreteValue>, int> discreteValueIds;
            std::unordered_map<std::shared_ptr<Statistics::FragilityValue>, int> fragilityValueIds;
            std::unordered_map<std::shared_ptr<Statistics::ContributingStochast>, int> contributingStochastIds;
            std::unordered_map<std::shared_ptr<Models::Message>, int> messageIds;
        };
    }
}
