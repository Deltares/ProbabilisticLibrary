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

        void ZModel::invoke(std::shared_ptr<ModelSample> sample)
        {
            if (this->zLambda == nullptr)
            {
                throw Reliability::probLibException("callback function not set or released");
            }

            sample->threadId = omp_get_thread_num();
            this->zLambda(sample);

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
                this->modelRuns += (int)samples.size();
            }
        }

        double ZModel::getBeta(std::shared_ptr<ModelSample> sample, double beta)
        {
            return this->zBetaLambda(sample, beta);
        }
    }
}



