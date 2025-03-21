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
#include <vector>
#include <functional>

#include "ModelInputParameter.h"
#include "ModelSample.h"
#include "RunSettings.h"
#include "ZValueConverter.h"
#include "SampleRepository.h"

namespace Deltares
{
    namespace Models
    {
        typedef std::function<void(std::shared_ptr<ModelSample>)> ZLambda;
        typedef std::function<void(std::vector<std::shared_ptr<ModelSample>>)> ZMultipleLambda;
        typedef std::function<double(std::shared_ptr<ModelSample>, double beta)> ZBetaLambda;

        typedef void (*ZValuesCallBack)(double* data, int size, double* outputValues);
        typedef void (*ZValuesMultipleCallBack)(int arraySize, double** data, int inputSize, double** outputValues);
        typedef void (*EmptyCallBack)();

        class ZModel
        {
        public:
            ZModel(ZLambda zLambda, ZMultipleLambda zMultipleLambda = nullptr)
            {
                this->zLambda = zLambda;
                this->zMultipleLambda = zMultipleLambda;
            }

            ZModel(ZValuesCallBack zValuesLambda, ZValuesMultipleCallBack zValuesMultipleLambda = nullptr)
            {
                this->zLambda = this->getLambdaFromZValuesCallBack(zValuesLambda);
                if (zValuesMultipleLambda != nullptr)
                {
                    this->zMultipleLambda = this->getLambdaFromZValuesMultipleCallBack(zValuesMultipleLambda);
                }
            }

            void setMultipleCallback(ZValuesMultipleCallBack multipleCallBack)
            {
                this->zMultipleLambda = this->getLambdaFromZValuesMultipleCallBack(multipleCallBack);
            }

            /**
             * \brief Name of the model
             */
            std::string name = "";

            /**
             * \brief Input parameters of the model
             */
            std::vector<std::shared_ptr<ModelInputParameter>> inputParameters;

            /**
             * \brief Output parameters of the model
             */
            std::vector<std::shared_ptr<ModelInputParameter>> outputParameters;

            /**
             * \brief Makes the model ready for invocations
             */
            void initializeForRun();

            void setRunMethod(EmptyCallBack runMethod)
            {
                this->runMethod = runMethod;
            }

            /**
             * \brief The index of the underlying model values if the model returns an array or tuple
             */
            int Index = 0;

            /**
             * \brief Calculates the z-value of a sample based on a calculated sample
             */
            std::shared_ptr<ZValueConverter> zValueConverter = std::make_shared<ZValueConverter>();

            void setBetaLambda(ZBetaLambda zBetaLambda)
            {
                this->zBetaLambda = zBetaLambda;
            }

            void releaseCallBacks();

            /**
             * \brief defines the maximum number of parallel processes or threads
             * \param maxProcesses Maximum number of parallel processes or threads
             */
            void setMaxProcesses(int maxProcesses);

            /**
             * \brief Calculates a sample
             */
            void invoke(std::shared_ptr<ModelSample> sample);

            /**
             * \brief Calculates a number of samples
             */
            void invoke(std::vector<std::shared_ptr<ModelSample>> samples);

            double getBeta(std::shared_ptr<ModelSample> sample, double beta);

            bool canCalculateBeta()
            {
                return this->zBetaLambda != nullptr;
            }

            int getModelRuns()
            {
                return modelRuns;
            }

            void resetModelRuns()
            {
                modelRuns = 0;
            }

        private:
            ZLambda zLambda = nullptr;
            ZMultipleLambda zMultipleLambda = nullptr;
            ZBetaLambda zBetaLambda = nullptr;
            EmptyCallBack runMethod = nullptr;
            int maxProcesses = 1;
            int modelRuns = 0;
            int inputParametersCount = 0;
            int outputParametersCount = 0;
            ZLambda getLambdaFromZValuesCallBack(ZValuesCallBack zValuesLambda);
            ZMultipleLambda getLambdaFromZValuesMultipleCallBack(ZValuesMultipleCallBack zValuesMultipleLambda);

            /**
             * \brief Calculates a sample
             */
            void invokeLambda(std::shared_ptr<ModelSample> sample);

            /**
             * \brief Calculates a number of samples
             */
            void invokeMultipleLambda(std::vector<std::shared_ptr<ModelSample>>& samples);

            /**
             * \brief The minimum calculation time in ms for storing it in the repository
             */
            long long minRepoCalculationTime = 1;

            /**
             * \brief Indicates whether calculation time should be measured
             */
            bool measureCalculationTime = true;

            /**
             * \brief Indicates the number of samples from which the calculation time is measured
             */
            int measuredCalculationTimes = 0;

            /**
             * \brief Indicates the maximum number of samples from which the calculation time will be measured
             */
            int maxMeasuredCalculationTimes = 100;

            /**
             * \brief Indicates whether samples should be saved in repository
             */
            bool useSampleRepository = false;

            /**
             * \brief Holds calculated samples
             */
            SampleRepository repository = SampleRepository();
        };
    }
}

