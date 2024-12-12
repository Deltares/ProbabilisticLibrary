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
#include <cmath>
#include "LinearRootFinder.h"
#include "../NumericSupport.h"

namespace Deltares
{
    namespace Numeric
    {
        double LinearRootFinder::CalculateValue(double xLow, double xHigh, double target, RootFinderMethod function)
        {
            double lowValue = function(xLow);
            double highValue = function(xHigh);

            // Initialize linear search method
            auto low = XValue(xLow, lowValue);
            auto high = XValue(xHigh, highValue);
            return CalculateValue(low, high, target, function);
        }

        double LinearRootFinder::CalculateValue(XValue low, XValue high, double target, RootFinderMethod function)
        {
            constexpr double windowLimit = 0.05;
            constexpr double solutionLimit = 0.2;

            if (low.X > high.X)
            {
                std::swap(low, high);
            }

            bool ascending = low.Value < high.Value;
            bool descending = low.Value > high.Value;

            auto solution = interpolate(low, high, target, function);

            // Linear search method
            int iterations = 0;
            double previousWindow = std::numeric_limits<double>::max();
            double previousSolution = std::numeric_limits<double>::max();
            bool bisect = false;

            while (std::abs(solution.Value - target) > tolerance && iterations < maxIterations)
            {
                if (solution.X < low.X)
                {
                    high = low;
                    low = solution;
                }
                else if (solution.X > high.X)
                {
                    low = high;
                    high = solution;
                }
                else if (solution.Value < target && high.Value < target)
                {
                    high = solution;
                }
                else if (solution.Value > target && high.Value > target)
                {
                    high = solution;
                }
                else if (solution.Value < target && high.Value > target)
                {
                    low = solution;
                }
                else if (solution.Value > target && high.Value < target)
                {
                    low = solution;
                }

                ascending |= low.Value < high.Value;
                descending |= low.Value > high.Value;

                if (ascending && descending)
                {
                    return nan("");
                }

                // if both the solution window end and the solution do not become significantly smaller, switch over to bisection

                double window = high.X - low.X;
                bisect |= std::abs((solution.Value - target) / (previousSolution - target)) > solutionLimit && window / previousWindow > 1 - windowLimit;

                previousSolution = solution.Value;

                if (bisect)
                {
                    solution = bisection(low, high, function);
                }
                else
                {
                    solution = interpolate(low, high, target, function);
                }

                if (low.X < solution.X && high.X > solution.X && std::abs(high.X - low.X) < tolerance)
                {
                    target = solution.Value;
                }

                previousWindow = window;

                iterations++;
            }

            if (std::abs(solution.Value - target) > tolerance)
            {
                return nan("");
            }

            return solution.X;
        }

        XValue LinearRootFinder::interpolate(const XValue& low, const XValue& high, double target, const RootFinderMethod& function)
        {
            double x = low.X + ((low.Value - target) / (low.Value - high.Value)) * (high.X - low.X);
            double xValue = function(x);
            auto result = XValue(x, xValue);
            return result;
        }

        XValue LinearRootFinder::bisection(const XValue& low, const XValue& high, const RootFinderMethod& function)
        {
            double x = (low.X + high.X) / 2;
            double xValue = function(x);
            auto result = XValue(x, xValue);
            return result;
        }
    }
}


