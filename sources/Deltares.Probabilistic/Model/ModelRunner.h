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
#include "../Logging/Message.h"
#include "../Proxies/ProxySettings.h"
#include "ModelSample.h"
#include "ProgressIndicator.h"
#include "../Uncertainty/CorrelationMatrixBuilder.h"
#include "../Uncertainty/UncertaintyResult.h"
#include "../Sensitivity/SensitivityResult.h"

namespace Deltares::Reliability
{
    class LimitStateFunction;
}

namespace Deltares::Models
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
        std::shared_ptr<Proxies::ProxySettings> ProxySettings = std::make_shared<Proxies::ProxySettings>();

        void initializeForRun();
        void clear();
        void clearLists();
        void releaseCallBacks() const;
        void useProxy(bool useProxy);
        void updateStochastSettings(const std::shared_ptr<Reliability::StochastSettingsSet>& settings);
        void setSampleProvider(const std::shared_ptr<SampleProvider>& sample_provider);
        double getZValue(const std::shared_ptr<Sample>& sample);
        std::vector<double> getZValues(const std::vector<std::shared_ptr<Sample>>& samples);
        double getBeta(const std::shared_ptr<Sample>& sample) const;
        bool canCalculateBeta() const;
        int getStochastCount() const;
        int getVaryingStochastCount() const;
        bool shouldExitPrematurely(const std::vector<std::shared_ptr<Sample>>& samples) const;
        void removeTask(int iterationIndex) const;

        void reportResult(const std::shared_ptr<Reliability::ReliabilityReport>& report);
        void reportProgress(int step, int maxSteps, double reliability = std::nan(""), double convergence = std::nan("")) const;
        void reportMessage(Logging::MessageType type, std::string text);

        void doTextualProgress(ProgressType type, const std::string& text) const;
        bool isVaryingStochast(int index) const;
        std::shared_ptr<Reliability::DesignPoint> getDesignPoint(const std::shared_ptr<Sample>& sample, double beta, const std::shared_ptr<Reliability::ConvergenceReport>& convergenceReport = nullptr, const std::string& identifier = "") const;
        Uncertainty::UncertaintyResult getUncertaintyResult(const std::shared_ptr<Statistics::Stochast>& stochast) const;
        Sensitivity::SensitivityResult getSensitivityResult() const;
        std::shared_ptr<Models::ModelSample> getModelSample(const std::shared_ptr<Sample>& sample) const;
        std::shared_ptr<Models::ModelSample> getModelSampleFromType(Statistics::RunValuesType type) const;
        std::vector<double> getOnlyVaryingValues(const std::vector<double>& values) const;

        void setDirectionModel(const ZBetaLambda& zBetaLambda) const;
        void setShouldExitFunction(ShouldExitLambda shouldExitFunction) { this->shouldExitFunction = shouldExitFunction; }
        void setShouldInvertFunction(ShouldInvertLambda shouldInvertFunction) { this->shouldInvertFunction = shouldInvertFunction; }
        void setRemoveTaskFunction(RemoveTaskLambda removeTaskFunction) { this->removeTaskFunction = removeTaskFunction; }
        void runDesignPoint(const std::shared_ptr<Reliability::DesignPoint>& designPoint);
        std::shared_ptr<Sample> getSampleFromStochastPoint(const std::shared_ptr<Models::StochastPoint>& stochastPoint) const;
        void registerSample(const std::shared_ptr<Uncertainty::CorrelationMatrixBuilder>& correlationMatrixBuilder, const std::shared_ptr<Sample>& sample) const;
        void updateVariableSample(std::vector<double>& xValues, std::vector<double>& originalValues) const;
        Evaluation getEvaluationFromType(Statistics::RunValuesType type) const;

        bool haveSampleValuesChanged() const { return uConverter->haveSampleValuesChanged(); }
        void setAllowRepository(bool proxyModel) const;
        Evaluation getEvaluation(const std::shared_ptr<Sample>& sample) const;
    private:
        std::shared_ptr<ZModel> zModel;
        std::shared_ptr<UConverter> uConverter;
        int runDesignPointCounter = 1;
        bool usingProxy = false;
        std::vector<std::shared_ptr<Reliability::ReliabilityResult>> reliabilityResults;
        std::vector<std::shared_ptr<Evaluation>> evaluations;
        std::vector< std::shared_ptr<Logging::Message>> messages;
        std::shared_ptr<ProgressIndicator> progressIndicator = nullptr;

        static Evaluation getEvaluationFromSample(const std::shared_ptr<ModelSample>& sample);

        void registerEvaluation(const std::shared_ptr<ModelSample>& sample);

        std::shared_ptr<SampleProvider> sampleProvider = nullptr;

        ShouldExitLambda shouldExitFunction = nullptr;
        ShouldInvertLambda shouldInvertFunction = nullptr;
        RemoveTaskLambda removeTaskFunction = nullptr;

        Utils::Locker* locker = nullptr;
    };
}

