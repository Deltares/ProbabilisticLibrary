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
#include "../Deltares.Probabilistic/Statistics/StandardNormal.h"
#include "stringHelper.h"

using namespace Deltares::Statistics;

/// <summary>
/// wrapper for getUFromQ
/// </summary>
/// <param name="q"> the q input value </param>
/// <param name="error_indication"> extra output for indication of an error situation </param>
/// <returns> the beta value </returns>
extern "C"
double betaFromQ(const double q, tError* error_indication)
{
    double beta = 0.0;
    try
    {
        beta = StandardNormal::getUFromQ(q);
        error_indication->errorCode = 0;
    }
    catch (const std::exception& e)
    {
        const std::string s = e.what();
        fillErrorMessage(*error_indication, s);
    }
    return beta;
}

/// <summary>
/// wrapper for getQFromU
/// </summary>
/// <param name="beta"> the beta input value </param>
/// <returns> the q value </returns>
extern "C"
double qFromBeta(const double beta)
{
    return StandardNormal::getQFromU(beta);
}

/// <summary>
/// wrapper for getPFromU
/// </summary>
/// <param name="beta"> the beta input value </param>
/// <returns> the p value </returns>
extern "C"
double pFromBeta(const double beta)
{
    return StandardNormal::getPFromU(beta);
}

/// <summary>
/// wrapper for getPQFromU
/// </summary>
/// <param name="beta"> the beta input value </param>
/// <param name="p"> the p output value </param>
/// <param name="q"> the q output value </param>
/// <returns> the p value </returns>
extern "C"
void pqFromBeta(const double beta, double* p, double* q)
{
    StandardNormal::getPQfromU(beta, *p, *q);
}

/// <summary>
/// wrapper for getTFromU
/// </summary>
/// <param name="beta"> the beta input value </param>
/// <returns> the return time </returns>
extern "C"
double returnTimeFromBeta(const double beta)
{
    return StandardNormal::getTFromU(beta);
}

/// <summary>
/// wrapper for getFreqFromU
/// </summary>
/// <param name="beta"> the beta input value </param>
/// <returns> the frequency </returns>
extern "C"
double freqFromBeta(const double beta)
{
    return StandardNormal::getFreqFromU(beta);
}

/// <summary>
/// wrapper for getLogQFromU
/// </summary>
/// <param name="beta"> the beta input value </param>
/// <returns> the log(q) </returns>
extern "C"
double logQFromBeta(const double beta)
{
    return StandardNormal::getLogQFromU(beta);
}
