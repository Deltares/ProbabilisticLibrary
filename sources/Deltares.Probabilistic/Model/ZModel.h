// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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

#include "ModelSample.h"
#include "RunSettings.h"

inline double Test() { return 2.4; }

namespace Deltares
{
    namespace Models
    {
        typedef std::function<void(std::shared_ptr<ModelSample>)> ZLambda;
        typedef std::function<void(std::vector<std::shared_ptr<ModelSample>>)> ZMultipleLambda;
        typedef std::function<double(std::shared_ptr<ModelSample>, double beta)> ZBetaLambda;

        typedef double (*ZValuesCallBack)(double* data, int size);

        class ZModel
        {
        private:
            ZLambda zLambda = nullptr;
            ZMultipleLambda zMultipleLambda = nullptr;
            ZBetaLambda zBetaLambda = nullptr;
            int maxProcesses = 1;
            int modelRuns = 0;
            bool countRunsLambda = true;

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
        };
    }
}

