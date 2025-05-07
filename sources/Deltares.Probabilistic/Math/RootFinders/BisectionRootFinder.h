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
#include "RootFinder.h"
namespace Deltares::Numeric
{
    enum class  DirectionType { Positive, Negative, Zero };

    class BisectionRootFinder : public RootFinder
    {
    public:
        BisectionRootFinder() = default;
        explicit BisectionRootFinder(double tol) : tolerance(tol) {}
        explicit BisectionRootFinder(double ztol, double xtol) : tolerance(ztol), xTolerance(xtol) {}
        explicit BisectionRootFinder(double ztol, double xtol, int maxIter) : tolerance(ztol), xTolerance(xtol), maxIterationsPerLoop(maxIter) {}
        double CalculateValue(double minStart, double maxStart, double resultValue, const RootFinderMethod& function) override;
        XValue CalculateValue(XValue minStart, XValue maxStart, double resultValue, const RootFinderMethod& function) override;
    private:
        static DirectionType getDirection(XValue xvalue1, XValue xvalue2);
        static double getRelativeDifference(double minValue, double maxValue);
        void UpdateMinMax(XValue& minStart, XValue& maxStart, double resultValue, const RootFinderMethod& function) const;
        double tolerance = 0.001;
        double xTolerance = 1E-6;
        int maxIterationsPerLoop = 250; // a maximum per while loop
    };
}
