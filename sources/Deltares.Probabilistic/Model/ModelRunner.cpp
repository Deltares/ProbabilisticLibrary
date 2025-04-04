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
#include "../Math/NumericSupport.h"
#include "../Statistics/Stochast.h"
#include <cmath>

#include "ModelSample.h"

#if __has_include(<format>)
#include <format>
#else
#include "../Utils/probLibString.h"
#endif

namespace Deltares
{
    namespace Models
    {
        int ModelRunner::getVaryingStochastCount()
        {
            return this->uConverter->getVaryingStochastCount();
        }

        int ModelRunner::getStochastCount()
        {
            return this->uConverter->getStochastCount();
        }

        bool ModelRunner::isVaryingStochast(int index)
        {
            return this->uConverter->isVaryingStochast(index);
        }

        void ModelRunner::updateStochastSettings(std::shared_ptr<Reliability::StochastSettingsSet> settings)
        {
            this->uConverter->updateStochastSettings(settings);
            this->sampleProvider = std::make_shared<SampleProvider>(settings);
        }

        void ModelRunner::setSampleProvider(std::shared_ptr<SampleProvider> sampleProvider)
        {
            this->sampleProvider = sampleProvider;
        }


        void ModelRunner::initializeForRun()
        {
            this->uConverter->initializeForRun();
            this->zModel->setMaxProcesses(this->Settings->MaxParallelProcesses);
            this->zModel->resetModelRuns();

            this->zModel->initializeForRun();

            if (this->locker == nullptr)
            {
                this->locker = new Utils::Locker();
            }

            if (sampleProvider == nullptr)
            {
                sampleProvider = std::make_shared<SampleProvider>(this->uConverter->getVaryingStochastCount(), this->uConverter->getStochastCount());
            }
        }

        void ModelRunner::clear()
        {
            clearLists();
            this->runDesignPointCounter = 1;
        }

        void ModelRunner::clearLists()
        {
            this->reliabilityResults.clear();
            this->evaluations.clear();
            this->messages.clear();
        }

        void ModelRunner::releaseCallBacks()
        {
            if (this->zModel != nullptr)
            {
                zModel->releaseCallBacks();
            }
        }

        std::shared_ptr<ModelSample> ModelRunner::getModelSample(std::shared_ptr<Sample> sample)
        {
            std::vector<double> xValues = this->uConverter->getXValues(sample);

            // create a sample with values in x-space
            std::shared_ptr<ModelSample> xSample = sampleProvider->getModelSample(xValues);

            xSample->AllowProxy = sample->AllowProxy;
            xSample->IterationIndex = sample->IterationIndex;
            xSample->threadId = sample->threadId;
            xSample->Weight = sample->Weight;
            xSample->IsRestartRequired = sample->IsRestartRequired;
            xSample->Beta = sample->getBeta();

            return xSample;
        }

        std::shared_ptr<ModelSample> ModelRunner::getModelSampleFromType(Statistics::RunValuesType type)
        {
            std::vector<double> xValues = this->uConverter->getValuesFromType(type);

            // create a sample with values in x-space
            std::shared_ptr<ModelSample> xSample = sampleProvider->getModelSample(xValues);

            return xSample;
        }

        /**
         * \brief Calculates a sample
         * \param sample Sample to be calculated
         * \return Z-value of the sample
         */
        double ModelRunner::getZValue(std::shared_ptr<Sample> sample)
        {
            std::shared_ptr<ModelSample> xSample = getModelSample(sample);

            this->zModel->invoke(xSample);

            registerEvaluation(xSample);

            sample->Z = xSample->Z;

            return sample->Z;
        }

        /**
         * \brief Calculates a sample
         * \param sample Sample to be calculated
         * \return Evaluation report of the sample calculation
         */
        Evaluation* ModelRunner::getEvaluation(std::shared_ptr<Sample> sample)
        {
            std::shared_ptr<ModelSample> xSample = getModelSample(sample);

            this->zModel->invoke(xSample);

            Evaluation* evaluation = getEvaluationFromSample(xSample);

            return evaluation;
        }

        /**
         * \brief Calculates a sample
         * \param type Run values type
         * \return Evaluation report of the sample calculation
         */
        Evaluation* ModelRunner::getEvaluationFromType(Statistics::RunValuesType type)
        {
            std::shared_ptr<ModelSample> xSample = getModelSampleFromType(type);

            this->zModel->invoke(xSample);

            Evaluation* evaluation = getEvaluationFromSample(xSample);

            return evaluation;
        }

        /**
         * \brief Runs the model in the design point
         * \param designPoint design point to be calculated
         * \return Z-value of the sample
         */
        void ModelRunner::runDesignPoint(std::shared_ptr<Reliability::DesignPoint> designPoint)
        {
            std::shared_ptr<Sample> sample = designPoint->getSample();
            std::shared_ptr<ModelSample> xSample = getModelSample(sample);

            xSample->ExtendedLogging = this->Settings->ExtendedLoggingAtDesignPoint;
            xSample->LoggingCounter = runDesignPointCounter++;

            this->zModel->invoke(xSample);

            registerEvaluation(xSample);

            for (size_t i = 0; i < xSample->Values.size(); i++)
            {
                designPoint->Alphas[i]->X = xSample->Values[i];
            }
        }

        /**
         * \brief Calculates a number of samples
         * \param samples Samples to be calculated
         * \return Z-values of the samples
         */
        std::vector<double> ModelRunner::getZValues(std::vector<std::shared_ptr<Sample>> samples)
        {
            std::vector<std::shared_ptr<ModelSample>> xSamples;

            for (size_t i = 0; i < samples.size(); i++)
            {
                xSamples.push_back(getModelSample(samples[i]));
            }

            this->zModel->invoke(xSamples);

            std::vector<double> zValues(xSamples.size());

            for (size_t i = 0; i < xSamples.size(); i++)
            {
                registerEvaluation(xSamples[i]);

                samples[i]->Z = xSamples[i]->Z;
                samples[i]->AllowProxy = xSamples[i]->AllowProxy;
                samples[i]->IsRestartRequired = xSamples[i]->IsRestartRequired;
                zValues[i] = xSamples[i]->Z;
            }

            return zValues;
        }

        /**
         * \brief Sets a callback which calculates the beat in a certain direction
         * \param zBetaLambda Callback 
         */
        void ModelRunner::setDirectionModel(ZBetaLambda zBetaLambda) const
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
        double ModelRunner::getBeta(std::shared_ptr<Sample> sample)
        {
            std::shared_ptr<ModelSample> xSample = getModelSample(sample);

            return this->zModel->getBeta(xSample, sample->getBeta());
        }

        Evaluation* ModelRunner::getEvaluationFromSample(std::shared_ptr<ModelSample> sample)
        {
            Evaluation* evaluation = new Evaluation();

            evaluation->Z = sample->Z;
            evaluation->Beta = sample->Beta;
            evaluation->Iteration = sample->IterationIndex;
            evaluation->InputValues = sample->Values;
            evaluation->OutputValues = sample->OutputValues;
            evaluation->Tag = sample->Tag;

            return evaluation;
        }

        /**
         * \brief Registers an evaluation for a calculated sample
         * \param sample Calculated sample
         */
        void ModelRunner::registerEvaluation(std::shared_ptr<ModelSample> sample)
        {
            if (this->Settings->SaveEvaluations)
            {
                std::shared_ptr<Evaluation> evaluation = std::shared_ptr<Evaluation>(getEvaluationFromSample(sample));

                if (this->Settings->MaxParallelProcesses > 1) 
                {
                    locker->lock();
                    this->evaluations.push_back(evaluation);
                    locker->unlock();
                }
                else
                {
                    this->evaluations.push_back(evaluation);
                }
            }
        }

        /**
         * \brief Indicates whether the reliability algorithm should be stopped
         * \param samples Already calculated samples
         * \return Indication
         */
        bool ModelRunner::shouldExitPrematurely(std::vector<std::shared_ptr<Sample>> samples)
        {
            for (std::shared_ptr<Sample> sample : samples)
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
        void ModelRunner::removeTask(int iterationIndex)
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
        void ModelRunner::reportResult(std::shared_ptr<Reliability::ReliabilityReport> report)
        {
            if (Settings->SaveConvergence)
            {
                bool hasPreviousReport = this->reliabilityResults.size() > 0;

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

            if (this->progressIndicator != nullptr)
            {
                const double progress = Numeric::NumericSupport::Divide(report->Step, report->MaxSteps);

                double convergence = report->ConvBeta;
                if (std::isnan(convergence))
                {
                    convergence = report->Variation;
                }

                this->reportProgress(report->Step, report->MaxSteps, report->Reliability, convergence);

                this->progressIndicator->doDetailedProgress(report->Step, report->Loop, report->Reliability, convergence);
            }
        }

        void ModelRunner::reportProgress(int step, int maxSteps, double reliability, double convergence)
        {
            if (this->progressIndicator != nullptr)
            {
                const double progress = Numeric::NumericSupport::Divide(step, maxSteps);
                this->progressIndicator->doProgress(progress);

#ifdef __cpp_lib_format
                auto text = std::format("{}/{}", step, maxSteps);
                if (!std::isnan(reliability) || !std::isnan(convergence))
                {
                    text = text + std::format(", Reliability = {:.3f}, Convergence = {:.3f}", reliability, convergence);
                }
#else
                auto text = std::to_string(step) + "/" + std::to_string(maxSteps);
                if (!std::isnan(reliability) || !std::isnan(convergence))
                {
                    auto pl = Deltares::Reliability::probLibString();
                    text = text + ", Reliability = " + pl.double2str(reliability);
                }
#endif

                this->progressIndicator->doTextualProgress(ProgressType::Detailed, text);
            }
        }

        void ModelRunner::reportMessage(MessageType type, std::string text)
        {
            if (Settings->SaveMessages && this->messages.size() < (size_t)this->Settings->MaxMessages && type >= this->Settings->LowestMessageType)
            {
                this->messages.push_back(std::make_shared<Message>(type, text));
            }
        }

        void ModelRunner::doTextualProgress(ProgressType type, std::string text)
        {
            if (this->progressIndicator != nullptr) 
            {
                this->progressIndicator->doTextualProgress(type, text);
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
        std::shared_ptr<Reliability::DesignPoint> ModelRunner::getDesignPoint(std::shared_ptr<Sample> sample, double beta, std::shared_ptr<Reliability::ConvergenceReport> convergenceReport, std::string identifier)
        {
            std::unique_ptr<Evaluation> evaluation = nullptr;
            if (this->uConverter->haveSampleValuesChanged())
            {
                evaluation = std::unique_ptr<Evaluation>(this->getEvaluation(sample->getSampleAtBeta(beta)));
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
                if (evaluation != nullptr)
                {
                    stochastPoint->Alphas[i]->X = evaluation->InputValues[i];
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

            for (size_t i = 0; i < this->reliabilityResults.size(); i++)
            {
                designPoint->ReliabililityResults.push_back(this->reliabilityResults[i]);
            }

            for (size_t i = 0; i < this->evaluations.size(); i++)
            {
                designPoint->Evaluations.push_back(this->evaluations[i]);
            }

            for (size_t i = 0; i < this->messages.size(); i++)
            {
                designPoint->Messages.push_back(this->messages[i]);
            }

            return designPoint;
        }

        /**
         * \brief Gets the result of a sensitivity calculation
         * \param stochast Stochast in the sensitivity result
         * \return Sensitivity result
         */
        Sensitivity::SensitivityResult ModelRunner::getSensitivityResult(std::shared_ptr<Statistics::Stochast> stochast) const
        {
            auto result = Sensitivity::SensitivityResult();

            result.stochast = stochast;

            for (const auto& evaluation : evaluations)
            {
                result.evaluations.push_back(evaluation);
            }

            for (const auto& message : messages)
            {
                result.messages.push_back(message);
            }

            return result;
        }

        void  ModelRunner::registerSample(std::shared_ptr<Sensitivity::CorrelationMatrixBuilder> correlationMatrixBuilder, std::shared_ptr<Sample> sample)
        {
            this->uConverter->registerSample(correlationMatrixBuilder, sample);
        }

        std::vector<double> ModelRunner::getOnlyVaryingValues(std::vector<double> values)
        {
            return this->uConverter->getVaryingValues(values);
        }

        void ModelRunner::updateVariableSample(std::vector<double>& xValues, std::vector<double>& originalValues)
        {
            this->uConverter->updateVariableSample(xValues, originalValues);
        }
    }
}



