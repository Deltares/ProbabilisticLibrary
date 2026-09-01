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

#include <thread>
#include <mutex>

#include "BaseHandler.h"
#include "ProjectEntries.h"
#include "../Reliability/ReliabilityProject.h"
#include "../Model/RunProject.h"

#include "Handlers/ModelHandlers.h"
#include "Handlers/ProbabilityValueHandler.h"
#include "Handlers/SensitivityResultHandler.h"
#include "Handlers/SensitivityValueHandler.h"
#include "Handlers/UncertaintyResultHandler.h"
#include "Handlers/ReliabilityHandlers.h"
#include "Handlers/SensitivitySettingsHandler.h"
#include "Handlers/UncertaintySettingsHandler.h"
#include "Handlers/SensitivityProjectHandler.h"
#include "Handlers/StatisticsHandlers.h"
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

        ModelHandlers modelHandlers;
        StatisticsHandlers statisticsHandlers;
        ReliabilityHandlers reliabilityHandlers;

    protected:
        virtual std::shared_ptr<Reliability::DesignPointIds> GetDesignPointIds(int id);
    private:

        ObjectHandlerAdmin admin;
        std::thread::id mainThreadId = std::this_thread::get_id();

        bool isMultiThread = false;
        bool isMultiThreadDetected = false;
        std::mutex mtx;
        std::vector<int> destroyObjects;

        void InitializeHandlers();

        UncertaintyResultHandler uncertaintyResultHandler;
        SensitivityResultHandler sensitivityResultHandler;
        SensitivityValueHandler sensitivityValueHandler;
        SensitivitySettingsHandler sensitivitySettingsHandler;
        UncertaintySettingsHandler uncertaintySettingsHandler;
        SensitivityProjectHandler sensitivityProjectHandler;
        UncertaintyProjectHandler uncertaintyProjectHandler;

        std::unordered_map<ObjectType, ObjectHandler*> handlers;

        std::shared_ptr<Models::ModelProject> GetProject(int id);
        void DestroyObjects();
    };
}
