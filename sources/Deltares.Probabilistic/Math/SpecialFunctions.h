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

namespace Deltares::Numeric
{
    class SpecialFunctions
    {
    public:
        static double getGamma(const double x);
        static double getGammaUpperRegularized(const double a, const double x);
        static double getGammaLowerRegularized(const double a, const double x);
        static double getBeta(double a, double b);
        static double getBetaRegularized(double a, double b, double x);
        static double getBetaIncomplete(double a, double b, double x);
    private:
        static double getGammaSeries(const double a, const double x);
        static double getGammaContinuedFraction(const double a, const double x);
        static double getLogGamma(const double x);

        static inline std::vector<double> cof =
        { 76.18009172947146, -86.50532032941677, 24.01409824083091,
         -1.231739572450155,  .1208650973866179e-2,  -.5395239384953e-5 };
    };
}


