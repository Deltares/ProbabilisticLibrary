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
#include "../Deltares.Probabilistic/Statistics/StandardNormal.h"
#include "stringHelper.h"

using namespace Deltares::Statistics;

extern "C"
void betafromqc(double * q, double * beta, tError * ierr)
{
    try
    {
        *beta = StandardNormal::getUFromQ(*q);
        ierr->errorCode = 0;
    }
    catch (const std::exception& e)
    {
        std::string s = e.what();
        fillErrorMessage(*ierr, s);
    }
}

extern "C"
double qfrombeta(double * beta)
{
    return StandardNormal::getQFromU(*beta);
}

extern "C"
double pfrombeta(double* beta)
{
    return StandardNormal::getPFromU(*beta);
}

extern "C"
void pqfrombeta(double* beta, double* p, double* q)
{
    StandardNormal::getPQfromU(*beta, *p, *q);
}

extern "C"
void returntimefrombeta(double *beta, double *returnTime)
{
    *returnTime =StandardNormal::ReturnTimeFromBeta(*beta);
}

extern "C"
void freqfrombeta(double *beta, double *freq)
{
    *freq = StandardNormal::FreqFromBeta(*beta);
}

extern "C"
void logqfrombeta(double *beta, double*logQ)
{
    *logQ = StandardNormal::LogQFromBeta(*beta);
}
