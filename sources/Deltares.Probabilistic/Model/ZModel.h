// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once
#include <limits>
#include <vector>
#include <functional>

#include "ModelInputParameter.h"
#include "ModelSample.h"
#include "RunSettings.h"

namespace Deltares
{
    namespace Models
    {
        typedef std::function<void(std::shared_ptr<ModelSample>)> ZLambda;
        typedef std::function<void(std::vector<std::shared_ptr<ModelSample>>)> ZMultipleLambda;
        typedef std::function<double(std::shared_ptr<ModelSample>, double beta)> ZBetaLambda;

        typedef double (*ZValuesCallBack)(double* data, int size);

        typedef void (*ZEmptyCallBack)();

        class ZModel
        {
        public:
            ZModel(ZLambda zLambda, ZMultipleLambda zMultipleLambda = nullptr)
            {
                this->zLambda = zLambda;
                this->zMultipleLambda = zMultipleLambda;
            }

            ZModel(ZValuesCallBack zValuesLambda)
            {
                ZLambda calcValuesLambda = [zValuesLambda](std::shared_ptr<ModelSample> sample)
                {
                    sample->Z = (*zValuesLambda)(sample->Values.data(), (int)sample->Values.size());
                };

                this->zLambda = calcValuesLambda;
            }

            /**
             * \brief Name of the model
             */
            std::string Name = "";

            /**
             * \brief The index of the underlying model values if the model returns an array or tuple
             */
            int Index = 0;

            void setBetaLambda(ZBetaLambda zBetaLambda)
            {
                this->zBetaLambda = zBetaLambda;
            }

            void releaseCallBacks();

            void setMaxProcesses(int maxProcesses);

            void invoke(std::shared_ptr<ModelSample> sample);

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

            /**
             * \brief Gets the name corresponding to the Index in this class
             */
            std::string getIndexedName()
            {
                if (this->Index < this->outputParameters.size())
                {
                    return this->outputParameters[this->Index]->name;
                }
                else
                {
                    return this->Name;
                }
            }

            std::vector<std::shared_ptr<ModelInputParameter>> inputParameters;
            std::vector<std::shared_ptr<ModelInputParameter>> outputParameters;

        private:
            ZLambda zLambda = nullptr;
            ZMultipleLambda zMultipleLambda = nullptr;
            ZBetaLambda zBetaLambda = nullptr;
            int maxProcesses = 1;
            int modelRuns = 0;
            bool countRunsLambda = true;
        };
    }
}

