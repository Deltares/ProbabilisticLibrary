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

#include <functional>

#include "RootFinder.h"

namespace Deltares
{
    namespace Numeric
    {
        class LinearRootFinder : public RootFinder
        {
        public:
            /// <summary>
            /// constructor
            /// </summary>
            /// <param name="tol"> z tolerance</param>
            /// <param name="maxIter"> maximum number of iterations </param>
            LinearRootFinder(double tol, int maxIter) : tolerance(tol), maxIterations(maxIter) {}

            /// <summary>
            /// Calculates the input value, which would lead to a result value when invoked for a given function. Extrapolation is allowed.
            /// </summary>
            /// <param name="xLow">Minimum start value</param>
            /// <param name="xHigh">Maximum start value</param>
            /// <param name="target">The result value</param>
            /// <param name="function">The function</param>
            /// <returns>The value which would lead to the result value when invoked for the function</returns>
            double CalculateValue(double xLow, double xHigh, double target, RootFinderMethod function) override;
            double CalculateValue(XValue low, XValue high, double target, RootFinderMethod function) override;

        private:
            static XValue interpolate(const XValue& low, const XValue& high, double target, const RootFinderMethod& function);
            static XValue bisection(const XValue& low, const XValue& high, const RootFinderMethod& function);
            const double tolerance = 0.001;
            int maxIterations;
        };
    }
}

