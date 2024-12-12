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
#include <memory>
#include "stringHelper.h"
#include "../Deltares.Probabilistic.CWrapper/createDistribution.h"

using namespace Deltares::Reliability;

extern "C"
void calculatedistributioninverse_c(double *u, double *y, EnumDistributions *type, double p[], tError *ierr)
{
    try
    {
        auto s = createDistribution::createValid(*type, p);
        *y = s->getXFromU(*u);
        ierr->errorCode = 0;
    }
    catch (const std::exception& e)
    {
        ierr->errorCode = -1;
        std::string s = e.what();
        fillErrorMessage(*ierr, s);
    }
}

extern "C"
void calculatedistribution_c(double* x, double* u, EnumDistributions * type, double p[], tError * ierr)
{
    try
    {
        auto s = createDistribution::createValid(*type, p);
        *u = s->getUFromX(*x);
        ierr->errorCode = 0;
    }
    catch (const std::exception& e)
    {
        ierr->errorCode = -1;
        std::string s = e.what();
        fillErrorMessage(*ierr, s);
    }
}


