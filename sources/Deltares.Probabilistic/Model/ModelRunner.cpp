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
#include "ModelRunner.h"
#include "ModelSample.h"
#include "../Math/NumericSupport.h"
#include "../Statistics/Stochast.h"
#include <cmath>

#include "../Proxies/ProxyModel.h"
#include <format>

#include "ModelSampleStorage.h"
#include "../Reliability/FragilityCurve.h"

namespace Deltares::Models
{
    int ModelRunner::getVaryingStochastCount() const
    {
        return this->uConverter->getVaryingStochastCount();
    }

    int ModelRunner::getStochastCount() const
    {
        return this->uConverter->getStochastCount();
    }

    bool ModelRunner::isVaryingStochast(int index) const
    {
        return this->uConverter->isVaryingStochast(index);
    }

    void ModelRunner::initializeForRun()
    {
        this->uConverter->initializeForRun();
        BaseModelRunner::initializeForRun();
    }

    void ModelRunner::clear()
    {
        BaseModelRunner::clear();
        clearLists();
        this->runDesignPointCounter = 1;
    }

    void ModelRunner::clearLists()
    {
        this->reliabilityResults.clear();
        BaseModelRunner::clearLists();
    }

    void ModelRunner::useProxy(bool useProxy)
    {
        if (useProxy && !usingProxy)
        {
            zModel = std::make_shared<Proxies::ProxyModel>(this->zModel);
            std::dynamic_pointer_cast<Proxies::ProxyModel>(zModel)->settings = this->ProxySettings;
            std::dynamic_pointer_cast<Proxies::ProxyModel>(zModel)->setConverter(this->uConverter);
            usingProxy = true;
        }
        else if (!useProxy && usingProxy)
        {
            zModel = std::dynamic_pointer_cast<Proxies::ProxyModel>(zModel)->getZModel();
            usingProxy = false;
        }
        else
        {
            // nothing to do
        }
    }

    void ModelRunner::setSampleProvider(const std::shared_ptr<SampleProvider>& sample_provider)
    {
        sampleProvider = sample_provider;
    }

    void ModelRunner::updateStochastSettings(const std::shared_ptr<Reliability::StochastSettingsSet>& settings)
    {
        uConverter->updateStochastSettings(settings);
        sampleProvider = std::make_shared<SampleProvider>(*settings);
    }

    ModelSample ModelRunner::getModelSample(Sample& sample) const
    {
        std::vector<double> xValues = this->uConverter->getXValues(sample);

        // create a sample with values in x-space
        ModelSample xSample = SampleProvider::getModelSample(xValues);

        xSample.AllowProxy = sample.AllowProxy;
        xSample.IterationIndex = sample.IterationIndex;
        xSample.threadId = sample.threadId;
        xSample.Weight = sample.Weight;
        xSample.IsRestartRequired = sample.IsRestartRequired;
        xSample.Beta = sample.getBeta();
        xSample.OutputValues.resize(this->zModel->outputParameters.size());

        return xSample;
    }

    ModelSample ModelRunner::getModelSampleFromType(Statistics::RunValuesType type) const
    {
        std::vector<double> xValues = this->uConverter->getValuesFromType(type);

        // create a sample with values in x-space
        ModelSample xSample = SampleProvider::getModelSample(xValues);
        xSample.OutputValues.resize(this->zModel->outputParameters.size());

        return xSample;
    }

    /**
     * \brief Calculates a sample
     * \param sample Sample to be calculated
     * \return Z-value of the sample
     */
    double ModelRunner::getZValue(Sample& sample)
    {
        ModelSample xSample = getModelSample(sample);

        this->zModel->invoke(xSample);

        registerEvaluation(xSample);

        sample.Z = xSample.Z;

        return sample.Z;
    }

    /**
     * \brief Calculates a sample
     * \param sample Sample to be calculated
     * \return Evaluation report of the sample calculation
     */
    Evaluation ModelRunner::getEvaluation(Sample& sample) const
    {
        ModelSample xSample = getModelSample(sample);

        this->zModel->invoke(xSample);

        Evaluation evaluation = getEvaluationFromSample(xSample);

        return evaluation;
    }

    /**
     * \brief Calculates a sample
     * \param type Run values type
     * \return Evaluation report of the sample calculation
     */
    Evaluation ModelRunner::getEvaluationFromType(Statistics::RunValuesType type) const
    {
        ModelSample xSample = getModelSampleFromType(type);

        this->zModel->invoke(xSample);

        Evaluation evaluation = getEvaluationFromSample(xSample);

        return evaluation;
    }

    Sample ModelRunner::getSampleFromStochastPoint(const std::shared_ptr<Models::StochastPoint>& stochastPoint) const
    {
        return this->uConverter->getSampleFromStochastPoint(stochastPoint);
    }

    /**
     * \brief Runs the model in the design point
     * \param designPoint design point to be calculated
     * \return Z-value of the sample
     */
    void ModelRunner::runDesignPoint(const std::shared_ptr<Reliability::DesignPoint>& designPoint)
    {
        Sample sample = designPoint->getSample();
        ModelSample xSample = getModelSample(sample);

        xSample.ExtendedLogging = this->Settings->ExtendedLoggingAtDesignPoint;
        xSample.LoggingCounter = runDesignPointCounter++;

        this->zModel->invoke(xSample);

        registerEvaluation(xSample);

        for (size_t i = 0; i < xSample.Values.size(); i++)
        {
            designPoint->Alphas[i]->X = xSample.Values[i];
        }
    }

    /**
     * \brief Calculates a number of samples
     * \param samples Samples to be calculated
     * \return Z-values of the samples
     */
    std::vector<double> ModelRunner::getZValues(std::vector<Sample*>& samples)
    {
        std::vector<ModelSample*> xSamples;
        ModelSampleStorage storage = ModelSampleStorage(samples.size());

        xSamples.reserve(samples.size());

        for (auto sample : samples)
        {
            ModelSample xSample = getModelSample(*sample);
            xSamples.push_back(storage.keep(xSample));
        }

        this->zModel->invoke(xSamples);

        std::vector<double> zValues(xSamples.size());

        for (size_t i = 0; i < xSamples.size(); i++)
        {
            registerEvaluation(*xSamples[i]);

            samples[i]->Z = xSamples[i]->Z;
            samples[i]->AllowProxy = xSamples[i]->AllowProxy;
            samples[i]->IsRestartRequired = xSamples[i]->IsRestartRequired;
            zValues[i] = xSamples[i]->Z;
        }

        return zValues;
    }

    /**
     * \brief Sets a callback which calculates the beta in a certain direction
     * \param zBetaLambda Callback
     */
    void ModelRunner::setDirectionModel(const ZBetaLambda& zBetaLambda) const
    {
        this->zModel->setBetaLambda(zBetaLambda);
    }

    /**
     * \brief Indicates whether this model runner can calculate the beta (distance to limit state) in a given direction
     * \return Indication
     */
    bool ModelRunner::canCalculateBeta() const
    {
        return this->zModel->canCalculateBeta();
    }

    /**
     * \brief Gets the beta (distance to limit state) in a given direction
     * \param sample Sample indicating the direction
     * \return Beta
     */
    double ModelRunner::getBeta(Sample& sample) const
    {
        ModelSample xSample = getModelSample(sample);

        return this->zModel->getBeta(xSample);
    }

    /**
     * \brief Indicates whether the reliability algorithm should be stopped
     * \param samples Already calculated samples
     * \return Indication
     */
    bool ModelRunner::shouldExitPrematurely(const std::vector<Sample*>& samples) const
    {
        for (Sample* sample : samples)
        {
            if (sample->IsRestartRequired)
            {
                return true;
            }
        }

        if (shouldExitFunction != nullptr)
        {
            return shouldExitFunction(false);
        }

        return false;
    }

    /**
     * \brief Removes a task for further processing
     * \param iterationIndex Iteration index of the task
     */
    void ModelRunner::removeTask(int iterationIndex) const
    {
        if (this->removeTaskFunction != nullptr)
        {
            this->removeTaskFunction(iterationIndex);
        }
    }

    /**
     * \brief Registers intermediate results and provides progress information of a reliability calculation
     * \param report Intermediate results
     * \remark The intermediate results will be part of the design point of the reliability calculation
     */
    void ModelRunner::reportResult(const std::shared_ptr<Reliability::ReliabilityReport>& report)
    {
        if (Settings->SaveConvergence)
        {
            bool hasPreviousReport = !this->reliabilityResults.empty();

            std::shared_ptr<Reliability::ReliabilityResult> previousReport = nullptr;
            if (hasPreviousReport)
            {
                previousReport = this->reliabilityResults.back();
            }

            std::shared_ptr<Reliability::ReliabilityResult> result = std::make_shared<Reliability::ReliabilityResult>();
            result->Reliability = report->Reliability;
            result->ConvBeta = report->ConvBeta;
            result->Variation = report->Variation;
            result->Contribution = report->Contribution;
            result->Index = hasPreviousReport ? previousReport->Index + 1 : 0;

            if (report->ReportMatchesEvaluation && previousReport != nullptr)
            {
                std::shared_ptr<Reliability::ReliabilityResult> previousPreviousReport = this->reliabilityResults.size() > 1
                    ? this->reliabilityResults[this->reliabilityResults.size() - 2]
                    : nullptr;

                // remove the last result
                if (!previousReport->IsMeaningful(previousPreviousReport, result))
                {
                    this->reliabilityResults.pop_back();
                }
            }

            this->reliabilityResults.push_back(result);
        }

        if (canProgress())
        {
            double convergence = report->ConvBeta;
            if (std::isnan(convergence))
            {
                convergence = report->Variation;
            }

            this->reportProgress(report->Step, report->MaxSteps, report->Reliability, convergence);
            this->reportDetailedProgress(report->Step, report->Loop, report->Reliability, convergence);

        }
    }

    /**
     * \brief Gets the design point of a reliability calculation
     * \param sample Sample on which the design point is based
     * \param beta Reliability index
     * \param convergenceReport Convergence information, will be appended to design point
     * \param identifier Identifying text
     * \return Design point
     */
    std::shared_ptr<Reliability::DesignPoint> ModelRunner::getDesignPoint(Sample& sample, double beta, const std::shared_ptr<Reliability::ConvergenceReport>& convergenceReport, const std::string& identifier)
    {
        Evaluation evaluation;
        bool evaluationAssigned = false;

        if (this->uConverter->haveSampleValuesChanged())
        {
            Sample betaSample = sample.getSampleAtBeta(beta);
            evaluation = this->getEvaluation(betaSample);
            evaluationAssigned = true;
        }

        std::shared_ptr<StochastPoint> stochastPoint = uConverter->GetStochastPoint(sample, beta);

        if (this->shouldInvertFunction != nullptr)
        {
            for (size_t i = 0; i < stochastPoint->Alphas.size(); i++)
            {
                if (shouldInvertFunction(static_cast<int>(i)))
                {
                    stochastPoint->Alphas[i]->invert();
                }
            }
        }

        std::shared_ptr<Reliability::DesignPoint> designPoint = std::make_shared<Reliability::DesignPoint>();

        designPoint->Beta = stochastPoint->Beta;

        for (size_t i = 0; i < stochastPoint->Alphas.size(); i++)
        {
            if (evaluationAssigned)
            {
                stochastPoint->Alphas[i]->X = evaluation.InputValues[i];
            }
            designPoint->Alphas.push_back(stochastPoint->Alphas[i]);
        }

        designPoint->Identifier = identifier;
        designPoint->convergenceReport = convergenceReport;

        if (designPoint->convergenceReport != nullptr)
        {
            designPoint->convergenceReport->TotalModelRuns = this->zModel->getModelRuns();
        }
        this->zModel->resetModelRuns();

        for (const auto& reliabilityResult : this->reliabilityResults)
        {
            designPoint->ReliabilityResults.push_back(reliabilityResult);
        }

        CollectMessages(designPoint->Evaluations, designPoint->Messages);

        return designPoint;
    }

    /**
     * \brief Gets the result of an uncertainty calculation
     * \param stochast Stochast in the uncertainty result
     * \return Uncertainty result
     */
    Uncertainty::UncertaintyResult ModelRunner::getUncertaintyResult(const std::shared_ptr<Statistics::Stochast>& stochast) const
    {
        auto result = Uncertainty::UncertaintyResult();

        result.stochast = stochast;

        CollectMessages(result.evaluations, result.messages);

        return result;
    }

    /**
     * \brief Gets an empty result of a sensitivity calculation with stochast definition and runtime information
     */
    Sensitivity::SensitivityResult ModelRunner::getSensitivityResult() const
    {
        Sensitivity::SensitivityResult result = uConverter->getSensitivityResult();

        CollectMessages(result.evaluations, result.messages);

        return result;
    }

    void ModelRunner::registerSample(const std::shared_ptr<Uncertainty::CorrelationMatrixBuilder>& correlationMatrixBuilder, Sample& sample) const
    {
        this->uConverter->registerSample(correlationMatrixBuilder, sample);
    }

    std::vector<double> ModelRunner::getOnlyVaryingValues(const std::vector<double>& values) const
    {
        return this->uConverter->getVaryingValues(values);
    }
}



