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
                    for (size_t j = 0; j < this->outputParametersCount; j++)
                    {
                        outputValues[i][j] = std::nan("");
                    }
                }

                try
                {
                    (*zValuesMultipleLambda)(static_cast<int>(samples.size()), inputValues, this->inputParametersCount, outputValues);
                }
                catch (const std::exception e)
                {
                    // empty on purpose
                }

                for (size_t i = 0; i < samples.size(); i++)
                {
                    samples[i]->OutputValues.clear();
                    for (size_t j = 0; j < this->outputParametersCount; j++)
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
                this->outputParametersCount++;
            }

            this->zValueConverter->initialize(this->inputParameters, this->outputParameters);
        }

        void ZModel::invoke(std::shared_ptr<ModelSample> sample)
        {
            if (this->zLambda == nullptr)
            {
                throw Reliability::probLibException("callback function not set or released");
            }

            sample->threadId = omp_get_thread_num();
            this->zLambda(sample);

            this->zValueConverter->updateZValue(sample);

            if (countRunsLambda)
            {
                this->modelRuns++;
            }
        }

        void ZModel::invoke(std::vector<std::shared_ptr<ModelSample>> samples)
        {
            if (zMultipleLambda == nullptr)
            {
                this->countRunsLambda = false;

#pragma omp parallel for
                for (int i = 0; i < (int)samples.size(); i++)
                {
                    invoke(samples[i]);
                }

                this->modelRuns += (int)samples.size();
                this->countRunsLambda = true;
            }
            else
            {
                this->zMultipleLambda(samples);

                for (int i = 0; i < (int)samples.size(); i++)
                {
                    this->zValueConverter->updateZValue(samples[i]);
                }

                this->modelRuns += (int)samples.size();
            }
        }

        double ZModel::getBeta(std::shared_ptr<ModelSample> sample, double beta)
        {
            return this->zBetaLambda(sample, beta);
        }
    }
}



