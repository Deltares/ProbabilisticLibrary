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
    using enum DirectionType;

    DirectionType BisectionRootFinder::getDirection(XValue xvalue1, XValue xvalue2)
    {
        if (xvalue1.Value > xvalue2.Value && xvalue1.X > xvalue2.X)
        {
            return Positive;
        }
        else if (xvalue1.Value < xvalue2.Value && xvalue1.X > xvalue2.X)
        {
            return Negative;
        }
        else if (xvalue1.Value > xvalue2.Value && xvalue1.X < xvalue2.X)
        {
            return Negative;
        }
        else if (xvalue1.Value < xvalue2.Value && xvalue1.X < xvalue2.X)
        {
            return Positive;
        }
        else
        {
            return Zero;
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
        UpdateMinMax(minStart, maxStart, resultValue, function);

        auto direction = getDirection(minStart, maxStart);

        // Initialize bisection method
        double result = minStart.Value;
        double value = minStart.X;

        double step = (maxStart.X - minStart.X) / 2.0;
        double difference = std::fabs(resultValue - result);
        double xDifference = getRelativeDifference(minStart.X, maxStart.X);

        // Bisection method
        int cntFunctionCalls = 0;
        while (difference > tolerance && xDifference > xTolerance && cntFunctionCalls < maxIterationsPerLoop)
        {
            double prevValue = value;

            if (direction == Positive && resultValue > result)
            {
                value += step;
            }
            else if (direction == Positive && resultValue < result)
            {
                value -= step;
            }
            else if (direction == Negative && resultValue > result)
            {
                value -= step;
            }
            else if (direction == Negative && resultValue < result)
            {
                value += step;
            }

            step /= 2.0;

            result = function(value);
            cntFunctionCalls++;

            difference = std::fabs(resultValue - result);

            if (step < 1E-30)
            {
                difference = 0.0; // enforce quit
            }

            xDifference = getRelativeDifference(value, prevValue);
        }

        if (xDifference > xTolerance && std::fabs(resultValue - result) > tolerance)
        {
            return nan("");
        }

        return value;
    }

    // Extrapolate until target is between minStart and maxStart
    // first check whether result is high enough
    // second check for result is low enough
    void BisectionRootFinder::UpdateMinMax(XValue& minStart, XValue& maxStart, double resultValue, const RootFinderMethod& function) const
    {
        if (minStart.X > maxStart.X)
        {
            std::swap(minStart, maxStart);
        }

        DirectionType direction = getDirection(minStart, maxStart);
        const auto cmp_directions = std::vector{ Positive , Negative };

        for (const auto& cmp_direction : cmp_directions)
        {
            int cntFunctionCalls = 0;
            while (cntFunctionCalls < maxIterationsPerLoop)
            {
                const bool cmp_results = (
                    cmp_direction == Positive
                    ? minStart.Value < resultValue && maxStart.Value < resultValue
                    : minStart.Value > resultValue && maxStart.Value > resultValue);
                if (!cmp_results)
                {
                    break;
                }
                if (direction == Zero)
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
    }

}

