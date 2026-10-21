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
#include "ConvergenceResult.h"
#include "RunSettings.h"
#include "ZModel.h"
#include "ModelSample.h"
#include "ProgressIndicator.h"
#include "../Utils/Locker.h"
#include "../Logging/Message.h"

namespace Deltares::Models
{
    /**
     * \brief Facilitates running samples defined in x-space
     */
    class BaseModelRunner
    {
    public:
        /**
         * \brief Constructor
         * \param zModel The underlying model to run samples
         * \param progressIndicator Progress indicator to which progress messages are sent
         */
        BaseModelRunner(std::shared_ptr<ZModel> zModel, std::shared_ptr<ProgressIndicator> progressIndicator = nullptr)
        {
            this->zModel = zModel;
            this->progressIndicator = progressIndicator;
        }

        virtual ~BaseModelRunner()
        {
            delete this->locker;
        }

        /**
         * Settings applicable for running samples
         */
        std::shared_ptr<RunSettings> Settings = std::make_shared<RunSettings>();

        /**
         * Prepares the model runner for a number of runs executed within one of the algorithms
         */
        virtual void initializeForRun();

        virtual void clear();
        virtual void clearLists();

        void reportProgress(int step, int maxSteps, double reliability = std::nan(""), double convergence = std::nan("")) const;
        void reportMessage(Logging::MessageType type, std::string text);
        void reportDetailedProgress(int step, int loop, double reliability, double convergence) const;
        void reportResult(double minimum, int step, int maxSteps);
        void doTextualProgress(ProgressType type, const std::string& text) const;

        void invoke(ModelSample& sample);
        void invoke(std::vector<ModelSample*>& samples);

        void setAllowRepository(bool allowRepository) const;

        bool canCalculateBeta() const;
        double getBeta(ModelSample& sample) const;
        bool hasConstraint() const;
        double getConstraint(ModelSample& sample);

        int getModelRuns() const;
        void collect_messages(std::vector<std::shared_ptr<Evaluation>>& evaluations, std::vector<std::shared_ptr<Logging::Message>>& messages) const;
        void collect_convergence_results(std::vector<std::shared_ptr<ConvergenceResult>>& convergenceResults) const;

    protected:

        int getOutputParametersSize() const;
        Evaluation getEvaluationFromSample(ModelSample& sample) const;
        bool canProgress() const;

    private:
        std::shared_ptr<ZModel> zModel;

        std::vector<std::shared_ptr<Evaluation>> evaluations;
        std::vector<std::shared_ptr<Logging::Message>> messages;
        std::vector<std::shared_ptr<ConvergenceResult>> convergenceResults;

        std::shared_ptr<ProgressIndicator> progressIndicator = nullptr;

        void registerEvaluation(ModelSample& sample);

        Utils::Locker* locker = nullptr;
    };
}

