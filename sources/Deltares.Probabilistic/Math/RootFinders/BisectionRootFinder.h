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
#include "RootFinder.h"
namespace Deltares
{
    namespace Numeric
    {
        enum class  DirectionType { Positive, Negative, Zero };

        class BisectionRootFinder : public RootFinder
        {
        public:
            BisectionRootFinder() = default;
            explicit BisectionRootFinder(double tol) : tolerance(tol) {}
            explicit BisectionRootFinder(double ztol, double xtol) : tolerance(ztol), xTolerance(xtol) {}
            double CalculateValue(double minStart, double maxStart, double resultValue, RootFinderMethod function) override;
        private:
            static DirectionType getDirection(double value1, double value2, double result1, double result2);
            static double getRelativeDifference(double minValue, double maxValue);
            double tolerance = 0.001;
            double xTolerance = 1E-6;
            int maxIterations = 750;
        };
    }
}

