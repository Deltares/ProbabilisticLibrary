#include "BisectionRootFinder.h"


namespace Deltares
{
    namespace Numeric
    {
        enum DirectionType { Positive, Negative, Zero };

        DirectionType getDirection(double value1, double value2, double result1, double result2)
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

        double getRelativeDifference(double minValue, double maxValue)
        {
            return std::fabs(maxValue - minValue) / std::max(std::fabs(maxValue), std::fabs(minValue));
        }

        double BisectionRootFinder::CalculateValue(double minStart, double maxStart, double resultValue, double tolerance, RootFinderMethod function, StopMethod isStopped, double xTolerance)
        {
            if (isStopped == nullptr)
            {
                isStopped = [] {return false; };
            }

            if (minStart > maxStart)
            {
                std::swap(minStart, maxStart);
            }

            double minResult = function(minStart);
            double maxResult = function(maxStart);

            DirectionType direction = getDirection(minStart, maxStart, minResult, maxResult);

            // Extrapolate until target is between minStart and maxStart, first check whether result is high enough
            while (minResult < resultValue && maxResult < resultValue && !isStopped())
            {
                if (direction == DirectionType::Zero)
                {
                    double diff = maxStart - minStart;

                    maxStart += diff;
                    maxResult = function(maxStart);

                    minStart -= diff;
                    minResult = function(minStart);
                }
                else if (direction == DirectionType::Positive)
                {
                    maxStart += maxStart - minStart;
                    maxResult = function(maxStart);
                }
                else
                {
                    minStart -= maxStart - minStart;
                    minResult = function(minStart);
                }

                direction = getDirection(minStart, maxStart, minResult, maxResult);
            }

            // Extrapolate until target is between minStart and maxStart, check whether result is low enough
            while (minResult > resultValue && maxResult > resultValue && !isStopped())
            {
                if (direction == DirectionType::Zero)
                {
                    double diff = maxStart - minStart;

                    maxStart += diff;
                    maxResult = function(maxStart);

                    minStart -= diff;
                    minResult = function(minStart);
                }
                else if (direction == DirectionType::Negative)
                {
                    maxStart += maxStart - minStart;
                    maxResult = function(maxStart);
                }
                else
                {
                    minStart -= maxStart - minStart;
                    minResult = function(minStart);
                }

                direction = getDirection(minStart, maxStart, minResult, maxResult);
            }

            // Initialize bisection method
            double result = minResult;
            double value = minStart;

            double step = (maxStart - minStart) / 2;
            double difference = std::fabs(resultValue - result);
            double xDifference = getRelativeDifference(minStart, maxStart);

            // Bisection method
            while (difference > tolerance && xDifference > xTolerance && !isStopped())
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

                difference = std::fabs(resultValue - result);

                if (step < 1E-30)
                {
                    difference = 0; // enforce quit
                }

                xDifference = getRelativeDifference(value, prevValue);
            }

            if (xDifference > xTolerance && std::fabs(resultValue - result) > tolerance)
            {
                return NAN;
            }

            return value;
        }
    }
}

