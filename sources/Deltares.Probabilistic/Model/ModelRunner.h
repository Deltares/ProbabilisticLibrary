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

#include "Sample.h"
#include <vector>

#include "../Reliability/ReliabilityReport.h"
#include "../Reliability/ReliabilityResult.h"
#include "../Reliability/DesignPoint.h"
#include "../Reliability/StochastSettingsSet.h"
#include "../Utils/Locker.h"
#include "SampleProvider.h"
#include "RunSettings.h"
#include "UConverter.h"
#include "ZModel.h"
#include "Message.h"
#include "ModelSample.h"
#include "ProgressIndicator.h"
#include "../Uncertainty/CorrelationMatrixBuilder.h"
#include "../Uncertainty/UncertaintyResult.h"

namespace Deltares
{
    namespace Models
    {
        typedef std::function<bool(bool finalCall)> ShouldExitLambda;
        typedef std::function<void(int iterationIndex)> RemoveTaskLambda;
        typedef std::function<bool(int stochastIndex)> ShouldInvertLambda;

        class ModelRunner
        {
        public:
            ModelRunner(std::shared_ptr<ZModel> zModel, std::shared_ptr<UConverter>uConverter, std::shared_ptr<ProgressIndicator> progressIndicator = nullptr)
            {
                this->zModel = zModel;
                this->uConverter = uConverter;
                this->progressIndicator = progressIndicator;
            }

            ~ModelRunner()
            {
                delete this->locker;
            }

            std::shared_ptr<RunSettings> Settings = std::make_shared<RunSettings>();

            void initializeForRun();
            void clear();
            void clearLists();
            void releaseCallBacks();
            void updateStochastSettings(std::shared_ptr<Reliability::StochastSettingsSet> settings);
            void setSampleProvider(std::shared_ptr<SampleProvider> sampleProvider);
            double getZValue(std::shared_ptr<Sample> sample);
            std::vector<double> getZValues(std::vector<std::shared_ptr<Sample>> samples);
            double getBeta(std::shared_ptr<Sample> sample);
            bool canCalculateBeta() const;
            int getStochastCount();
            int getVaryingStochastCount();
            bool shouldExitPrematurely(std::vector<std::shared_ptr<Sample>> samples);
            void removeTask(int iterationIndex);

            void reportResult(std::shared_ptr<Reliability::ReliabilityReport> report);
            void reportProgress(int step, int maxSteps, double reliability = std::nan(""), double convergence = std::nan(""));
            void reportMessage(MessageType type, std::string text);

            void doTextualProgress(ProgressType type, std::string text);
            bool isVaryingStochast(int index);
            std::shared_ptr<Reliability::DesignPoint> getDesignPoint(std::shared_ptr<Sample> sample, double beta, std::shared_ptr<Reliability::ConvergenceReport> convergenceReport = nullptr, std::string identifier = "");
            Uncertainty::UncertaintyResult getSensitivityResult(std::shared_ptr<Statistics::Stochast> stochast) const;
            std::shared_ptr<Models::ModelSample> getModelSample(std::shared_ptr<Sample> sample);
            std::shared_ptr<Models::ModelSample> getModelSampleFromType(Statistics::RunValuesType type);
            std::vector<double> getOnlyVaryingValues(std::vector<double> values);

            void setDirectionModel(ZBetaLambda zBetaLambda) const;
            void setShouldExitFunction(ShouldExitLambda shouldExitFunction) { this->shouldExitFunction = shouldExitFunction; }
            void setShouldInvertFunction(ShouldInvertLambda shouldInvertFunction) { this->shouldInvertFunction = shouldInvertFunction; }
            void setRemoveTaskFunction(RemoveTaskLambda removeTaskFunction) { this->removeTaskFunction = removeTaskFunction; }
            void runDesignPoint(std::shared_ptr<Reliability::DesignPoint> designPoint);
            void registerSample(std::shared_ptr<Uncertainty::CorrelationMatrixBuilder> correlationMatrixBuilder, std::shared_ptr<Sample> sample);
            void updateVariableSample(std::vector<double>& xValues, std::vector<double>& originalValues);
            Evaluation getEvaluationFromType(Statistics::RunValuesType type);

            bool haveSampleValuesChanged() const { return uConverter->haveSampleValuesChanged(); }
            void setAllowRepository(bool proxyModel) const;
            Evaluation getEvaluation(std::shared_ptr<Sample> sample);
        private:
            std::shared_ptr<ZModel> zModel;
            std::shared_ptr<UConverter> uConverter;
            int runDesignPointCounter = 1;
            std::vector<std::shared_ptr<Reliability::ReliabilityResult>> reliabilityResults;
            std::vector<std::shared_ptr<Evaluation>> evaluations;
            std::vector< std::shared_ptr<Message>> messages;
            std::shared_ptr<ProgressIndicator> progressIndicator = nullptr;

            Evaluation getEvaluationFromSample(std::shared_ptr<ModelSample> sample);

            void registerEvaluation(std::shared_ptr<ModelSample> sample);

            std::shared_ptr<SampleProvider> sampleProvider = nullptr;

            ShouldExitLambda shouldExitFunction = nullptr;
            ShouldInvertLambda shouldInvertFunction = nullptr;
            RemoveTaskLambda removeTaskFunction = nullptr;

            Utils::Locker* locker = nullptr;
        };
    }
}

