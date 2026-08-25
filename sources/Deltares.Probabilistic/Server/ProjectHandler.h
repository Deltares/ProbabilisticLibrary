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
#include "../Reliability/FragilityCurve.h"
#include "../Reliability/FragilityCurveProject.h"
#include "../Reliability/FragilityCurveIntegrationSettings.h"
#include "../Reliability/LimitStateFunction.h"
#include "../Reliability/CombinedLimitStateFunction.h"
#include "../Reliability/Settings.h"
#include "../Reliability/ProbabilityLimitStateFunction.h"
#include "../Model/Evaluation.h"
#include "../Model/RunProject.h"
#include "../Model/RunProjectSettings.h"
#include "../Uncertainty/UncertaintyProject.h"
#include "../Uncertainty/SettingsS.h"
#include "../Sensitivity/SensitivityProject.h"
#include "../Sensitivity/SensitivitySettings.h"
#include "../Sensitivity/SensitivityResult.h"
#include "../Sensitivity/SensitivityValue.h"
#include "../Combine/CombineProject.h"
#include "../Combine/CombineSettings.h"
#include "../Combine/ExcludingCombineProject.h"
#include "../Combine/ExcludingCombineSettings.h"
#include "../Combine/LengthEffectProject.h"
#include "../Statistics/Stochast.h"
#include "Handlers/HandlerAdmin.h"

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

        int GetFragilityCurveId(const std::shared_ptr<Reliability::FragilityCurve>& fragilityCurve, int newId);
        int GetCorrelationMatrixId(const std::shared_ptr<Statistics::BaseCorrelation>& correlationMatrix, int newid);
        int GetDesignPointId(const std::shared_ptr<Reliability::DesignPoint>& designPoint, int newId);
        int GetAlphaId(const std::shared_ptr<Models::StochastPointAlpha>& alpha, int newId);
        int GetUncertaintyResultId(const std::shared_ptr<Uncertainty::UncertaintyResult>& result, int newId);
        int GetSensitivityResultId(const std::shared_ptr<Sensitivity::SensitivityResult>& result, int newId);
        int GetSensitivityValueId(const std::shared_ptr<Sensitivity::SensitivityValue>& result, int newId);
        int GetContributingStochastId(const std::shared_ptr<Statistics::ContributingStochast>& contributingStochast, int newId);
        int GetConditionalValueId(const std::shared_ptr<Statistics::VariableStochastValue>& conditionalValue, int newId);
        int GetSelfCorrelationMatrixId(const std::shared_ptr<Statistics::SelfCorrelationMatrix>& correlationMatrix, int newId);
        int GetStatus(const std::string& command) const;

        std::shared_ptr <Reliability::DesignPoint> GetDesignPoint(int id)
        {
            return designPoints[id];
        }
    protected:
        virtual std::shared_ptr<Reliability::DesignPointIds> GetDesignPointIds(int id);
    private:

        std::mutex mtx;
        HandlerAdmin admin;

        std::unordered_map<int, std::shared_ptr<Reliability::ReliabilityProject>> projects;
        std::unordered_map<int, std::shared_ptr<Reliability::CombinedLimitStateFunction>> combinedLimitStateFunctions;
        std::unordered_map<int, std::shared_ptr<Reliability::ProbabilityLimitStateFunction>> probabilityLimitStateFunctions;
        std::unordered_map<int, std::shared_ptr<Statistics::BaseCorrelation>> correlations;
        std::unordered_map<int, std::shared_ptr<Reliability::Settings>> settingsValues;
        std::unordered_map<int, std::shared_ptr<Reliability::StochastSettings>> stochastSettingsValues;
        std::unordered_map<int, std::shared_ptr<Models::StochastPoint>> stochastPoints;
        std::unordered_map<int, std::shared_ptr<Reliability::DesignPoint>> designPoints;
        std::unordered_map<int, std::shared_ptr<Models::StochastPointAlpha>> alphas;
        std::unordered_map<int, std::shared_ptr<Reliability::FragilityCurve>> fragilityCurves;
        std::unordered_map<int, std::shared_ptr<Reliability::FragilityCurveProject>> fragilityCurveProjects;
        std::unordered_map<int, std::shared_ptr<Reliability::FragilityCurveIntegrationSettings>> fragilityCurveSettings;
        std::unordered_map<int, std::shared_ptr<Reliability::CombineProject>> combineProjects;
        std::unordered_map<int, std::shared_ptr<Reliability::CombineSettings>> combineSettingsValues;
        std::unordered_map<int, std::shared_ptr<Reliability::ExcludingCombineProject>> excludingCombineProjects;
        std::unordered_map<int, std::shared_ptr<Reliability::ExcludingCombineSettings>> excludingCombineSettings;
        std::unordered_map<int, std::shared_ptr<Reliability::LengthEffectProject>> lengthEffectProjects;
        std::unordered_map<int, std::shared_ptr<Statistics::SelfCorrelationMatrix>> selfCorrelationMatrices;
        std::unordered_map<int, std::shared_ptr<Models::RunProject>> runProjects;
        std::unordered_map<int, std::shared_ptr<Models::RunProjectSettings>> runProjectSettings;
        std::unordered_map<int, std::shared_ptr<Sensitivity::SensitivityProject>> sensitivityProjects;
        std::unordered_map<int, std::shared_ptr<Sensitivity::SensitivitySettings>> sensitivitySettingsValues;
        std::unordered_map<int, std::shared_ptr<Sensitivity::SensitivityResult>> sensitivityResults;
        std::unordered_map<int, std::shared_ptr<Sensitivity::SensitivityValue>> sensitivityValues;
        std::unordered_map<int, std::shared_ptr<Uncertainty::UncertaintyProject>> uncertaintyProjects;
        std::unordered_map<int, std::shared_ptr<Uncertainty::SettingsS>> uncertaintySettingsValues;
        std::unordered_map<int, std::shared_ptr<Uncertainty::UncertaintyResult>> uncertaintyResults;

        std::unordered_map<std::shared_ptr<Reliability::CombinedLimitStateFunction>, int> combinedLimitStateFunctionIds;
        std::unordered_map<std::shared_ptr<Reliability::Settings>, int> settingsValuesIds;
        std::unordered_map<std::shared_ptr<Reliability::DesignPoint>, int> designPointIds;
        std::unordered_map<std::shared_ptr<Uncertainty::UncertaintyResult>, int> uncertaintyResultsIds;
        std::unordered_map<std::shared_ptr<Sensitivity::SensitivityResult>, int> sensitivityResultsIds;
        std::unordered_map<std::shared_ptr<Sensitivity::SensitivityValue>, int> sensitivityValuesIds;
        std::unordered_map<std::shared_ptr<Models::StochastPointAlpha>, int> alphaIds;
        std::unordered_map<std::shared_ptr<Reliability::FragilityCurve>, int> fragilityCurveIds;
        std::unordered_map<std::shared_ptr<Statistics::BaseCorrelation>, int> correlationIds;
        std::unordered_map<std::shared_ptr<Statistics::SelfCorrelationMatrix>, int> selfCorrelationIds;

        std::unordered_map <std::string, std::vector<double>> tempValues;

        double argValue = nan("");
        int tempIntValue = 0;

        std::shared_ptr<Models::ModelProject> GetProject(int id);
        std::shared_ptr<Models::ModelProjectSettings> GetSettings(int id);
        std::shared_ptr<Statistics::Stochast> GetStochast(int id);
        std::shared_ptr<Reliability::LimitStateFunction> GetLimitStateFunction(int id);

        bool IsSupported(ObjectType objectType);

    };
}
