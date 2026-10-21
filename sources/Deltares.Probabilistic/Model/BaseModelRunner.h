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
         * \param newModel The underlying model to run samples
         * \param newProgressIndicator Progress indicator to which progress messages are sent
         */
        explicit BaseModelRunner(const std::shared_ptr<ZModel>& newModel, const std::shared_ptr<ProgressIndicator>& newProgressIndicator = nullptr):
            zModel(newModel), progressIndicator(newProgressIndicator)
        {
        }

        virtual ~BaseModelRunner() = default;

        /**
         * Settings applicable for running samples
         */
        std::shared_ptr<RunSettings> Settings = std::make_shared<RunSettings>();

        /**
         * Prepares the model runner for a number of runs executed within one of the algorithms
         */
        virtual void initializeForRun();

        /**
         * \brief Resets counters and collections populated by an algorithm
         */
        virtual void clear();

        /**
         * \brief Sends progress during an algorithm
         * \param step Step in the algorithm
         * \param maxSteps Max steps  in the algorithm
         * \param reliability Reliability in the algorithm
         * \param convergence Convergence in the algorithm
         * \remark Pick up the progress by assigning a progress indicator in the constructor
         */
        void reportProgress(int step, int maxSteps, double reliability = std::nan(""), double convergence = std::nan("")) const;

        /**
         * \brief Registers a message during an algorithm
         * \param type The type of the message
         * \param text The text of the message
         * \remark Pick up the messages with the collect_messages method
         */
        void reportMessage(Logging::MessageType type, std::string text);

        /**
         * \brief Sends a detailed progress message during an algorithm
         * \param step Step in the algorithm
         * \param loop Loop counter in the algorithm
         * \param reliability Reliability in the algorithm
         * \param convergence Convergence in the algorithm
         * \remark Pick up the detailed progress by assigning a progress indicator in the constructor
         */
        void reportDetailedProgress(int step, int loop, double reliability, double convergence) const;

        /**
         * \brief Registers a convergence message during an algorithm
         * \param minimum The currently found minimum in during the algorithm
         * \param step Step in the algorithm
         * \param maxSteps Max steps  in the algorithm
         * \remark Pick up the convergence messages with the collect_convergence_results method
         */
        void reportConvergence(double minimum, int step, int maxSteps);

        /**
         * \brief Sends a textual progress message during an algorithm
         * \param type Type of the message
         * \param text Message text
         * \remark Pick up the textual progress by assigning a progress indicator in the constructor
         */
        void doTextualProgress(ProgressType type, const std::string& text) const;

        /**
         * \brief Calculates a sample (defined in x-space) with the underlying model
         * \param sample The sample to be calculated
         */
        void invoke(ModelSample& sample);

        /**
         * \brief Calculates a number of samples (defined in x-space) with the underlying model
         * \param samples The samples to be calculated
         */
        void invoke(std::vector<ModelSample*>& samples);

        /**
         * \brief Indicates whether the underlying model can generate a beta value of a sample
         * \return Indication
         */
        bool canCalculateBeta() const;

        /**
         * \brief Gets the beta value of a sample
         * \param sample The sample for which the beta value is calculated
         * \return Beta value
         */
        double getModelSampleBeta(ModelSample& sample) const;

        /**
         * \brief Indicates whether the underlying model can generate a constraint value of a sample
         * \return Indication
         */
        bool hasConstraint() const;

        /**
         * \brief Gets the constraint value of a sample
         * \param sample The sample for which the constraint value is calculated
         * \return Constraint value
         */
        double getConstraint(ModelSample& sample) const;

        /**
         * \brief Gets the number of performed sample calculations
         * \return Number of sample calculations
         * \remark Is reset by initializeForRun
         */
        int getModelRuns() const;

        /**
         * \brief Populates given collections with generated evaluations and messages during an algorithm
         * \param resultEvaluations Evaluations to be populated
         * \param resultMessages Messages to be populated
         */
        void collect_messages(std::vector<std::shared_ptr<Evaluation>>& resultEvaluations, std::vector<std::shared_ptr<Logging::Message>>& resultMessages) const;

        /**
         * \brief Populates a given collection with generated convergence messages during an algorithm
         * \param resultConvergenceResults Convergence results to be populated
         */
        void collect_convergence_results(std::vector<std::shared_ptr<ConvergenceResult>>& resultConvergenceResults) const;

    protected:
        /**
         * \brief Gets the size of the expected output array of the model calculation
         * \return Expected output size
         */
        size_t getOutputParametersSize() const;

        /**
         * \brief Creates an evaluation of an already calculated sample
         * \param sample The sample to create an evaluation from
         * \return Evaluation
         */
        Evaluation getEvaluationFromSample(const ModelSample& sample) const;

        /**
         * \brief Indicates whether progress messages can be sent
         * \return Indication
         */
        bool canProgress() const;

    private:
        std::shared_ptr<ZModel> zModel;

        std::vector<std::shared_ptr<Evaluation>> evaluations;
        std::vector<std::shared_ptr<Logging::Message>> messages;
        std::vector<std::shared_ptr<ConvergenceResult>> convergenceResults;

        std::shared_ptr<ProgressIndicator> progressIndicator = nullptr;

        /**
         * \brief Registers an evaluation for a calculated sample
         * \param sample Calculated sample
         */
        void registerEvaluation(ModelSample& sample);

        std::unique_ptr<Utils::Locker> locker = nullptr;
    };
}

