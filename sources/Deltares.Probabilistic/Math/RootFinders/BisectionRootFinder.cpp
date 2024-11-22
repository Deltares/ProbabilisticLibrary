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
#include "BisectionRootFinder.h"

namespace Deltares::Numeric
{
    DirectionType BisectionRootFinder::getDirection(XValue xvalue1, XValue xvalue2)
    {
        if (xvalue1.Value > xvalue2.Value && xvalue1.X > xvalue2.X)
        {
            return DirectionType::Positive;
        }
        else if (xvalue1.Value < xvalue2.Value && xvalue1.X > xvalue2.X)
        {
            return DirectionType::Negative;
        }
        else if (xvalue1.Value > xvalue2.Value && xvalue1.X < xvalue2.X)
        {
            return DirectionType::Negative;
        }
        else if (xvalue1.Value < xvalue2.Value && xvalue1.X < xvalue2.X)
        {
            return DirectionType::Positive;
        }
        else
        {
            return DirectionType::Zero;
        }
    }

    double BisectionRootFinder::getRelativeDifference(double minValue, double maxValue)
    {
        return std::fabs(maxValue - minValue) / std::max(std::fabs(maxValue), std::fabs(minValue));
    }

    double BisectionRootFinder::CalculateValue(double minStart, double maxStart, double resultValue, RootFinderMethod function)
    {
        double lowValue = function(minStart);
        double highValue = function(maxStart);

        // Initialize search method
        auto low = XValue(minStart, lowValue);
        auto high = XValue(maxStart, highValue);
        return CalculateValue(low, high, resultValue, function);
    }

    double BisectionRootFinder::CalculateValue(XValue minStart, XValue maxStart, double resultValue, RootFinderMethod function)
    {
        if (minStart.X > maxStart.X)
        {
            std::swap(minStart, maxStart);
        }

        int cntFunctionCalls = 0;

        DirectionType direction = getDirection(minStart, maxStart);
        const auto cmp_directions = std::vector<DirectionType> { DirectionType::Positive , DirectionType::Negative };

        for (const auto& cmp_direction : cmp_directions)
        {
            // Extrapolate until target is between minStart and maxStart
            // first check whether result is high enough
            // second check for result is low enough
            while (cntFunctionCalls < maxIterations)
            {
                const bool cmp_results = (
                    cmp_direction == DirectionType::Positive
                        ? minStart.Value < resultValue && maxStart.Value < resultValue
                        : minStart.Value > resultValue && maxStart.Value > resultValue);
                if (!cmp_results) break;
                if (direction == DirectionType::Zero)
                {
                    double diff = maxStart.X - minStart.X;

                    maxStart.X += diff;
                    maxStart.Value = function(maxStart.X);

                    minStart.X -= diff;
                    minStart.Value = function(minStart.X);
                    cntFunctionCalls += 2;
                }
                else if (direction == cmp_direction)
                {
                    maxStart.X += maxStart.X - minStart.X;
                    maxStart.Value = function(maxStart.X);
                    cntFunctionCalls++;
                }
                else
                {
                    minStart.X -= maxStart.X - minStart.X;
                    minStart.Value = function(minStart.X);
                    cntFunctionCalls++;
                }

                direction = getDirection(minStart, maxStart);
            }
        }

        // Initialize bisection method
        double result = minStart.Value;
        double value = minStart.X;

        double step = (maxStart.X - minStart.X) / 2;
        double difference = std::fabs(resultValue - result);
        double xDifference = getRelativeDifference(minStart.X, maxStart.X);

        // Bisection method
        while (difference > tolerance && xDifference > xTolerance && cntFunctionCalls < maxIterations)
        {
            double prevValue = value;

            if (direction == DirectionType::Positive && resultValue > result)
            {
                value += step;
            }
            else if (direction == DirectionType::Positive && resultValue < result)
            {
                value -= step;
            }
            else if (direction == DirectionType::Negative && resultValue > result)
            {
                value -= step;
            }
            else if (direction == DirectionType::Negative && resultValue < result)
            {
                value += step;
            }

            step = step / 2;

            result = function(value);
            cntFunctionCalls++;

            difference = std::fabs(resultValue - result);

            if (step < 1E-30)
            {
                difference = 0; // enforce quit
            }

            xDifference = getRelativeDifference(value, prevValue);
        }

        if (xDifference > xTolerance && std::fabs(resultValue - result) > tolerance)
        {
            return nan("");
        }

        return value;
    }
}

