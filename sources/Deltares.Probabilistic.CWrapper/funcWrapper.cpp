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
#include "funcWrapper.h"

#include "../Deltares.Probabilistic/Utils/probLibException.h"
#include <omp.h>

using namespace Deltares::Reliability;
using namespace Deltares::Models;

void funcWrapper::FDelegate(std::shared_ptr<ModelSample> s)
{
    designPointOptions dp = s->ExtendedLogging ? designPointOptions::dpOutTRUE : designPointOptions::dpOutFALSE;

    computationSettings compSetting{ dp, compId, s->threadId, s->LoggingCounter };
    tError e = tError();
    double result = zfunc(s->Values.data(), &compSetting, &e);
    if (e.errorCode != 0)
    {
        error_messages.push_back((e.errorMessage));
        s->Z = std::nan("");
    }
    else
    {
        s->Z = result;
    }
}

void funcWrapper::FDelegateParallel(std::vector<std::shared_ptr<ModelSample>> samples)
{
    auto errorMessagePerThread = std::vector<std::string>(omp_get_num_threads());

#pragma omp parallel for
    for (int i = 0; i < (int)samples.size(); i++)
    {
        computationSettings compSetting{ designPointOptions::dpOutFALSE, compId, omp_get_thread_num(), 1 };
        tError e = tError();
        double result = zfunc(samples[i]->Values.data(), &compSetting, &e);
        if (e.errorCode != 0)
        {
            errorMessagePerThread[omp_get_thread_num()] = e.errorMessage;
            samples[i]->Z = std::nan("");
        }
        else
        {
            samples[i]->Z = result;
        }
    }

    for (const auto& s : errorMessagePerThread)
    {
        if (s.length() > 0)
        {
            error_messages.push_back(s);
        }
    }
}

