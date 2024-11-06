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
#include <functional>
#include <cmath>

namespace Deltares
{
    namespace Numeric
    {
        typedef std::function<double(double)> RootFinderMethod;

        class RootFinder
        {

        public:
            virtual double CalculateValue(double xLow, double xHigh, double target, int maxIterations, RootFinderMethod function, double knownLowValue = nan(""), double knownHighValue = nan(""))
            {
                return nan("");
            }
        };
    }
}




