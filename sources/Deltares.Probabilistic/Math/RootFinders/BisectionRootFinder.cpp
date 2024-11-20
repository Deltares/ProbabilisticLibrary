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
    DirectionType BisectionRootFinder::getDirection(double value1, double value2, double result1, double result2)
    {
        if (result1 > result2 && value1 > value2)
        {
            return DirectionType::Positive;
        }
        else if (result1 < result2 && value1 > value2)
        {
            return DirectionType::Negative;
        }
        else if (result1 > result2 && value1 < value2)
        {
            return DirectionType::Negative;
        }
        else if (result1 < result2 && value1 < value2)
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
        if (minStart > maxStart)
        {
            std::swap(minStart, maxStart);
        }

        double minResult = function(minStart);
        double maxResult = function(maxStart);
        int cntFunctionCalls = 2;

        DirectionType direction = getDirection(minStart, maxStart, minResult, maxResult);
        const auto cmp_directions = std::vector<DirectionType> { DirectionType::Positive , DirectionType::Negative };

        for (int i = 0; i < 2; i++)
        {
            // Extrapolate until target is between minStart and maxStart
            // first check whether result is high enough
            // second check for result is low enough
            while (cntFunctionCalls < maxIterations)
            {
                const bool cmp_results = (i == 0 ? minResult < resultValue && maxResult < resultValue
                                                 : minResult > resultValue && maxResult > resultValue);
                if (!cmp_results) break;
                if (direction == DirectionType::Zero)
                {
                    double diff = maxStart - minStart;

                    maxStart += diff;
                    maxResult = function(maxStart);

                    minStart -= diff;
                    minResult = function(minStart);
                    cntFunctionCalls += 2;
                }
                else if (direction == cmp_directions[i])
                {
                    maxStart += maxStart - minStart;
                    maxResult = function(maxStart);
                    cntFunctionCalls++;
                }
                else
                {
                    minStart -= maxStart - minStart;
                    minResult = function(minStart);
                    cntFunctionCalls++;
                }

                direction = getDirection(minStart, maxStart, minResult, maxResult);
            }
        }

        // Initialize bisection method
        double result = minResult;
        double value = minStart;

        double step = (maxStart - minStart) / 2;
        double difference = std::fabs(resultValue - result);
        double xDifference = getRelativeDifference(minStart, maxStart);

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

