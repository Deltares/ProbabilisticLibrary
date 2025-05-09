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
#include "ZModel.h"

#include <chrono>
#include <omp.h>

#include "ModelSample.h"
#include "../Utils/probLibException.h"


namespace Deltares
{
    namespace Models
    {
        void ZModel::setMaxProcesses(int maxProcesses)
        {
            this->maxProcesses = maxProcesses;

            if (maxProcesses > 0)
            {
                omp_set_num_threads(maxProcesses);
            }
        }

        void ZModel::releaseCallBacks()
        {
            this->zLambda = nullptr;
            this->zMultipleLambda = nullptr;
        }

        ZLambda ZModel::getLambdaFromZValuesCallBack(ZValuesCallBack zValuesLambda)
        {
            ZLambda calcValuesLambda = [zValuesLambda, this](std::shared_ptr<ModelSample> sample)
            {
                double* inputValues = sample->Values.data();
                double* outputValues = new double[this->outputParametersCount];

                (*zValuesLambda)(inputValues, this->inputParametersCount, outputValues);

                sample->OutputValues.clear();
                for (size_t i = 0; i < this->outputParametersCount; i++)
                {
                    sample->OutputValues.push_back(outputValues[i]);
                }

                delete[] outputValues;
            };

            return calcValuesLambda;
        }

        ZMultipleLambda ZModel::getLambdaFromZValuesMultipleCallBack(ZValuesMultipleCallBack zValuesMultipleLambda)
        {
            ZMultipleLambda calcValuesLambda = [zValuesMultipleLambda, this](std::vector<std::shared_ptr<ModelSample>> samples)
            {
                double** inputValues = new double* [samples.size()];
                double** outputValues = new double* [samples.size()];
                for (size_t i = 0; i < samples.size(); i++)
                {
                    inputValues[i] = samples[i]->Values.data();
                    outputValues[i] = new double[this->outputParametersCount];
                    for (int j = 0; j < this->outputParametersCount; j++)
                    {
                        outputValues[i][j] = std::nan("");
                    }
                }

                try
                {
                    (*zValuesMultipleLambda)(static_cast<int>(samples.size()), inputValues, this->inputParametersCount, outputValues);
                }
                catch (const std::exception&)
                {
                    // empty on purpose
                }

                for (size_t i = 0; i < samples.size(); i++)
                {
                    samples[i]->OutputValues.clear();
                    for (int j = 0; j < this->outputParametersCount; j++)
                    {
                        samples[i]->OutputValues.push_back(outputValues[i][j]);
                    }
                    delete[] outputValues[i];
                }

                delete[] inputValues;
                delete[] outputValues;
            };

            return calcValuesLambda;
        }

        void ZModel::initializeForRun()
        {
            this->inputParametersCount = 0;
            this->outputParametersCount = 0;

            this->measureCalculationTime = true;
            this->useSampleRepository = false;
            this->measuredCalculationTimes = 0;

            for (std::shared_ptr<ModelInputParameter> parameter : this->inputParameters)
            {
                parameter->computationalIndex = this->inputParametersCount;
                if (parameter->isArray)
                {
                    this->inputParametersCount += parameter->arraySize;
                }
                else
                {
                    this->inputParametersCount++;
                }
            }

            for (std::shared_ptr<ModelInputParameter> parameter : this->outputParameters)
            {
                parameter->computationalIndex = this->outputParametersCount;
                if (parameter->isArray)
                {
                    this->outputParametersCount += parameter->arraySize;
                }
                else
                {
                    this->outputParametersCount++;
                }
            }

            this->zValueConverter->initialize(this->inputParameters, this->outputParameters);
        }

        void ZModel::invoke(std::shared_ptr<ModelSample> sample)
        {
            bool isSampleRepositoryAllowed = this->isRepositoryAllowed(sample);

            std::shared_ptr<ModelSample> alreadyExecutedSample = isSampleRepositoryAllowed ? repository.retrieveSample(sample) : nullptr;

            if (alreadyExecutedSample == nullptr)
            {
                if (measureCalculationTime)
                {
                    std::chrono::time_point started = std::chrono::high_resolution_clock::now();
                    invokeLambda(sample);
                    std::chrono::time_point done = std::chrono::high_resolution_clock::now();

                    long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();
                    if (elapsedTime > minRepoCalculationTime)
                    {
                        measureCalculationTime = false;
                        useSampleRepository = true;
                    }

                    this->measuredCalculationTimes++;
                    if (this->measuredCalculationTimes > this->maxMeasuredCalculationTimes)
                    {
                        measureCalculationTime = false;
                    }
                }
                else
                {
                    invokeLambda(sample);
                }

                if (useSampleRepository && !sample->UsedProxy)
                {
                    repository.registerSample(sample);
                }
            }
            else
            {
                sample->copyFrom(alreadyExecutedSample);
            }

            this->zValueConverter->updateZValue(sample);

            this->modelRuns++;
        }

        void ZModel::invokeLambda(std::shared_ptr<ModelSample> sample)
        {
            if (this->zLambda == nullptr)
            {
                throw Reliability::probLibException("callback function not set or released");
            }

            sample->threadId = omp_get_thread_num();
            this->zLambda(sample);
        }

        void ZModel::invokeMultipleLambda(std::vector<std::shared_ptr<ModelSample>>& samples)
        {
            if (zMultipleLambda == nullptr)
            {
#pragma omp parallel for
                for (int i = 0; i < static_cast<int>(samples.size()); i++)
                {
                    invokeLambda(samples[i]);
                }
            }
            else
            {
                this->zMultipleLambda(samples);
            }
        }

        void ZModel::invoke(std::vector<std::shared_ptr<ModelSample>> samples)
        {
            std::vector<std::shared_ptr<ModelSample>> executeSamples;
            for (std::shared_ptr<ModelSample> sample : samples)
            {
                bool isSampleRepositoryAllowed = this->isRepositoryAllowed(sample);

                std::shared_ptr<ModelSample> alreadyExecutedSample = isSampleRepositoryAllowed ? repository.retrieveSample(sample) : nullptr;

                if (alreadyExecutedSample == nullptr)
                {
                    executeSamples.push_back(sample);
                }
                else
                {
                    sample->copyFrom(alreadyExecutedSample);
                }
            }

            if (!executeSamples.empty())
            {
                if (this->measureCalculationTime)
                {
                    std::chrono::time_point started = std::chrono::high_resolution_clock::now();
                    invokeMultipleLambda(executeSamples);
                    std::chrono::time_point  done = std::chrono::high_resolution_clock::now();

                    long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();

                    if (elapsedTime >= static_cast<long long>(minRepoCalculationTime * executeSamples.size()))
                    {
                        useSampleRepository = true;
                        measureCalculationTime = false;
                    }

                    this->measuredCalculationTimes += static_cast<int>(executeSamples.size());
                    if (this->measuredCalculationTimes > this->maxMeasuredCalculationTimes)
                    {
                        measureCalculationTime = false;
                    }
                }
                else
                {
                    invokeMultipleLambda(executeSamples);
                }

                this->modelRuns += static_cast<int>(executeSamples.size());

                if (useSampleRepository)
                {
                    for (std::shared_ptr<ModelSample> sample : executeSamples)
                    {
                        if (!sample->UsedProxy)
                        {
                            repository.registerSample(sample);
                        }
                    }
                }
            }

            for (size_t i = 0; i < samples.size(); i++)
            {
                this->zValueConverter->updateZValue(samples[i]);
            }
        }

        bool ZModel::isRepositoryAllowed(const std::shared_ptr<ModelSample>& sample)
        {
            if (sample->AllowProxy)
            {
                return this->allowRepositoryForProxies;
            }
            else
            {
                return true;
            }
        }

        double ZModel::getBeta(std::shared_ptr<ModelSample> sample, double beta)
        {
            return this->zBetaLambda(sample, beta);
        }
    }
}



