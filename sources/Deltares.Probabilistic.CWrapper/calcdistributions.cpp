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
#include "stringHelper.h"
#include "../Deltares.Probabilistic.CWrapper/createDistribution.h"

using namespace Deltares::Reliability;

/// <summary>
/// wrapper for getXFromU
/// </summary>
/// <param name="u"> the u input value </param>
/// <param name="type"> enum for distribution type </param>
/// <param name="p"> array with 4 parameters defining the type </param>
/// <param name="error_indication"> extra output for indication of an error situation </param>
/// <returns> the x-value </returns>
extern "C"
double calculateDistributionInverse(const double u, const EnumDistributions type, double p[], tError* error_indication)
{
    double x = 0.0;
    try
    {
        auto s = createDistribution::createValid(type, p);
        x = s.getXFromU(u);
        error_indication->errorCode = 0;
    }
    catch (const std::exception& e)
    {
        error_indication->errorCode = -1;
        const std::string s = e.what();
        fillErrorMessage(*error_indication, s);
    }
    return x;
}

/// <summary>
/// wrapper for getUFromX
/// </summary>
/// <param name="x"> the x input value </param>
/// <param name="type"> enum for distribution type </param>
/// <param name="p"> array with 4 parameters defining the type </param>
/// <param name="error_indication"> extra output for indication of an error situation </param>
/// <returns> the u-value </returns>
extern "C"
double calculateDistribution(const double x, const EnumDistributions type, double p[], tError* error_indication)
{
    double u = 0.0;
    try
    {
        auto s = createDistribution::createValid(type, p);
        u = s.getUFromX(x);
        error_indication->errorCode = 0;
    }
    catch (const std::exception& e)
    {
        error_indication->errorCode = -1;
        const std::string s = e.what();
        fillErrorMessage(*error_indication, s);
    }
    return u;
}


