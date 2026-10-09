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

#include <vector>

#include "Evaluation.h"
#include "../Reliability/ReliabilityReport.h"
#include "../Reliability/ReliabilityResult.h"
#include "../Utils/Locker.h"
#include "RunSettings.h"
#include "ZModel.h"
#include "../Logging/Message.h"
#include "../Proxies/ProxySettings.h"
#include "ModelSample.h"
#include "ProgressIndicator.h"

namespace Deltares::Models
{
    class BaseModelRunner
    {
    public:
        BaseModelRunner(std::shared_ptr<ZModel> zModel, std::shared_ptr<ProgressIndicator> progressIndicator = nullptr)
        {
            this->zModel = zModel;
            this->progressIndicator = progressIndicator;
        }

        virtual ~BaseModelRunner()
        {
            delete this->locker;
        }

        std::shared_ptr<RunSettings> Settings = std::make_shared<RunSettings>();

        virtual void initializeForRun();
        virtual void clear();
        virtual void clearLists();

        void reportProgress(int step, int maxSteps, double reliability = std::nan(""), double convergence = std::nan("")) const;
        void reportMessage(Logging::MessageType type, std::string text);
        void reportDetailedProgress(int step, int loop, double reliability, double convergence) const;
        void doTextualProgress(ProgressType type, const std::string& text) const;

        void invoke(ModelSample& sample) const;
        void invoke(std::vector<ModelSample*>& samples) const;

        void setAllowRepository(bool allowRepository) const;

        bool canCalculateBeta() const;
        double getBeta(ModelSample& sample) const;

        int getModelRuns() const;
        void CollectMessages(std::vector<std::shared_ptr<Evaluation>>& evaluations, std::vector<std::shared_ptr<Logging::Message>>& messages) const;

    protected:

        std::shared_ptr<ZModel> getZModel();
        void setZModel(std::shared_ptr<ZModel> newZModel);

        int getOutputParametersSize() const;

        void registerEvaluation(ModelSample& sample);
        Evaluation getEvaluationFromSample(ModelSample& sample) const;
        bool canProgress() const;

    private:
        std::shared_ptr<ZModel> zModel;

        std::vector<std::shared_ptr<Evaluation>> evaluations;
        std::vector< std::shared_ptr<Logging::Message>> messages;
        std::shared_ptr<ProgressIndicator> progressIndicator = nullptr;

        Utils::Locker* locker = nullptr;
    };
}

