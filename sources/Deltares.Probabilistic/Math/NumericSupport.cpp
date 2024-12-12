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
#include "NumericSupport.h"

#include <cmath>
#include <functional>
#include <memory>

#include "Constants.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Numeric
    {
        double NumericSupport::GetSign(double value)
        {
            return value > 0.0 ? 1 : -1;
        }

        double NumericSupport::GetPrevailingSign(std::vector<double> values)
        {
            if (values.empty())
            {
                return 0;
            }
            else if (values.size() == 1)
            {
                return GetSign(values[0]);
            }
            else if (values.size() == 2)
            {
                return std::fabs(values[0]) >= std::fabs(values[1]) ? GetSign(values[0]) : GetSign(values[1]);
            }
            else
            {
                double sumNeg = 0;
                double sumPos = 0;

                for (auto value : values)
                {
                    if (value > 0) sumPos += value * value;
                    if (value < 0) sumNeg -= value * value;
                }

                return sumPos >= sumNeg ? 1.0 : -1.0;
            }
        }

        double NumericSupport::GetSquaredSum(const std::vector<double>& values)
        {
            double sum = 0;
            for (size_t i = 0; i < values.size(); i++)
            {
                sum += values[i] * values[i];
            }

            return sum;
        }

        double NumericSupport::GetLength(const std::vector<double>& values)
        {
            return sqrt(GetSquaredSum(values));
        }

        double NumericSupport::getDistance(const std::vector<double>& values1, const std::vector<double>& values2)
        {
            return sqrt(getDistance2(values1, values2));
        }

        double NumericSupport::getDistance2(const std::vector<double>& values1, const std::vector<double>& values2)
        {
            double sum = 0;

            for (size_t i = 0; i < values1.size(); i++)
            {
                const double diff = values1[i] - values2[i];
                sum += diff * diff;
            }

            return sum;
        }

        double* NumericSupport::getArray(double initialValue, int count)
        {
            double* newValues = new double[count];

            for (int i = 0; i < count; i++)
            {
                newValues[i] = initialValue;
            }

            return newValues;
        }

        double* NumericSupport::getCopy(double* values, int count)
        {
            double* newValues = new double[count];

            for (int i = 0; i < count; i++)
            {
                newValues[i] = values[i];
            }

            return newValues;
        }

        double* NumericSupport::getCopy(const std::vector<double>& values)
        {
            auto count = values.size();
            double* newValues = new double[count];

            for (size_t i = 0; i < count; i++)
            {
                newValues[i] = values[i];
            }

            return newValues;
        }

        double NumericSupport::Divide(int counter, int denominator)
        {
            return static_cast<double>(counter) / static_cast<double>(denominator);
        }

        bool NumericSupport::areEqual(double x1, double x2, double tolerance)
        {
            if (std::isnan(x1) && std::isnan(x2))
            {
                return true;
            }
            else
            {
                return fabs(x1 - x2) < tolerance;
            }
        }

        bool NumericSupport::isLess(double x1, double x2, double tolerance)
        {
            return x1 < x2 + tolerance;
        }

        bool NumericSupport::isGreater(double x1, double x2, double tolerance)
        {
            return x1 > x2 - tolerance;
        }

        CmpResult NumericSupport::compareDouble(const double x1, const double x2, const double tol)
        {
            if (areEqual(x1, x2, tol))
            {
                return CmpResult::Equal;
            }
            else if (x1 > x2)
            {
                return CmpResult::Greater;
            }
            else
            {
                return CmpResult::Smaller;
            }
        }

        bool NumericSupport::doublesAreEqual(const std::vector<double>& p1, const std::vector<double>& p2, const double tol)
        {
            if (p1.size() != p2.size())
            {
                throw Reliability::probLibException("lengths differ in doublesAreEqual.");
            }
            for (size_t i = 0; i < p1.size(); i++)
            {
                if (!areEqual(p1[i], p2[i], tol)) return false;
            }
            return true;
        }

        double NumericSupport::round(double value, int decimals)
        {
            double multiplier = 1;
            switch (decimals)
            {
            case 0: multiplier = 1; break;
            case 1: multiplier = 10; break;
            case 2: multiplier = 100; break;
            case 3: multiplier = 1000; break;
            case 4: multiplier = 10000; break;
            case 5: multiplier = 100000; break;
            case 6: multiplier = 1000000; break;
            case 7: multiplier = 10000000; break;
            case 8: multiplier = 100000000; break;
            case 9: multiplier = 1000000000; break;
            default: multiplier = std::pow(10.0, decimals);
            }

            return std::round(value * multiplier) / multiplier;
        }

        /// <summary>
        /// Retrieves qualitative information of a numeric value
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        DoubleType NumericSupport::getDoubleType(double value)
        {
            if (std::isnan(value))
            {
                return DoubleType::NaN;
            }
            else if (value > 0)
            {
                return DoubleType::Positive;
            }
            else if (value < 0)
            {
                return DoubleType::Negative;
            }
            else
            {
                return DoubleType::Zero;
            }
        }

        double NumericSupport::getSign(DoubleType doubleType)
        {
            switch (doubleType)
            {
            case DoubleType::Zero:
                return 0;
            case DoubleType::Negative:
                return -1;
            case DoubleType::Positive:
                return 1;
            case DoubleType::NaN:
                return nan("");
            default:
                throw Deltares::Reliability::probLibException("double type not supported");
            }
        }

        double NumericSupport::getFraction(double value, double fraction)
        {
            if (value == 0)
            {
                return fraction;
            }
            else
            {
                return fraction * std::abs(value);
            }
        }



        /// <summary>
        /// Converts cartesian coordinates to spherical coordinates
        /// </summary>
        /// <param name="cartesianCoordinates"></param>
        /// <returns></returns>
        /// <see cref="https://en.wikipedia.org/wiki/N-sphere#Spherical_coordinates"/>
        std::vector<double> NumericSupport::GetSphericalCoordinates(const std::vector<double>& cartesianCoordinates)
        {
            auto count = cartesianCoordinates.size();
            auto coordinates = std::vector<double>(count);

            coordinates[0] = GetSquaredSum(cartesianCoordinates);

            for (size_t i = 1; i < count; i++)
            {
                if (cartesianCoordinates[i - 1] != 0)
                {
                    double sum = 0;
                    for (size_t j = i - 1; j < count; j++)
                    {
                        sum += cartesianCoordinates[j] * cartesianCoordinates[j];
                    }

                    coordinates[i] = acos(cartesianCoordinates[i - 1] / sqrt(sum));
                }

                if (i == count - 1 && cartesianCoordinates[i] < 0)
                {
                    coordinates[i] = 2 * Deltares::Numeric::pi - coordinates[i];
                }
            }

            return coordinates;
        }

        /// <summary>
        /// Converts spherical coordinates to cartesian coordinates
        /// </summary>
        /// <param name="sphericalCoordinates"></param>
        /// <returns></returns>
        /// <see cref="https://en.wikipedia.org/wiki/N-sphere#Spherical_coordinates"/>
        std::vector<double> NumericSupport::GetCartesianCoordinates(const std::vector<double>& sphericalCoordinates)
        {
            auto count = sphericalCoordinates.size();
            auto coordinates = std::vector<double>(count);

            for (size_t i = 0; i < count; i++)
            {
                coordinates[i] = sphericalCoordinates[0];
                for (size_t j = 0; j < i; j++)
                {
                    coordinates[i] *= sin(sphericalCoordinates[j + 1]);
                }

                if (i < count - 1)
                {
                    coordinates[i] *= cos(sphericalCoordinates[i + 1]);
                }
            }

            return coordinates;
        }

        double NumericSupport::getMinimum(const std::vector<double>& values)
        {
            if (values.empty())
            {
                return nan("");
            }
            else
            {
                double min = values[0];
                for (double x : values)
                {
                    if (x < min)
                    {
                        min = x;
                    }
                }

                return min;
            }
        }

        double NumericSupport::getMaximum(const std::vector<double>& values)
        {
            if (values.empty())
            {
                return nan("");
            }
            else
            {
                double max = values[0];
                for (double x : values)
                {
                    if (x > max)
                    {
                        max = x;
                    }
                }

                return max;
            }
        }

        size_t NumericSupport::getLocationMinimum(const std::vector<double>& values)
        {
            if (values.empty())
            {
                throw Deltares::Reliability::probLibException("empty vector not allowed in getLocationMinimum");
            }
            else
            {
                size_t locmin = 0;
                for (size_t i = 1; i < values.size(); i++)
                {
                    if (values[i] < values[locmin])
                    {
                        locmin = i;
                    }
                }

                return locmin;
            }
        }

        size_t NumericSupport::getLocationMaximum(const std::vector<double>& values)
        {
            if (values.empty())
            {
                throw Deltares::Reliability::probLibException("empty vector not allowed in getLocationMaximum");
            }
            else
            {
                size_t locmax = 0;
                for (size_t i = 1; i < values.size(); i++)
                {
                    if (values[i] > values[locmax])
                    {
                        locmax = i;
                    }
                }

                return locmax;
            }
        }

        double NumericSupport::interpolate(double x, double minX, double minY, double maxX, double maxY, bool extrapolate, InterpolationType interpolationType)
        {
            if ((minX > maxX && interpolationType != InterpolationType::Harmonic) || (minX < maxX && interpolationType == InterpolationType::Harmonic))
            {
                std::swap(minX, maxX);
                std::swap(minY, maxY);
            }

            switch (interpolationType)
            {
            case InterpolationType::Linear:
                break;
            case InterpolationType::Logarithmic:
                x = std::log(x);
                minX = std::log(minX);
                maxX = std::log(maxX);
                break;
            case InterpolationType::Harmonic:
                x = 1.0 / x;
                minX = 1.0 / minX;
                maxX = 1.0 / maxX;
                break;
            default:
                throw Deltares::Reliability::probLibException("interpolation type not supported");
            }

            if (minY == maxY)
            {
                return minY;
            }
            else if (x < minX && !extrapolate)
            {
                return minY;
            }
            else if (x > maxX && !extrapolate)
            {
                return maxY;
            }
            else
            {
                const double xFraction = (x - minX) / (maxX - minX);
                return minY + (maxY - minY) * xFraction;
            }
        }

        double NumericSupport::interpolate(double x, const std::vector<double>& xValues, const std::vector<double>& yValues, bool extrapolate, InterpolationType interpolationType)
        {
            if (xValues.size() != yValues.size())
            {
                throw Deltares::Reliability::probLibException("XValues and YValues not of the same length");
            }

            if (xValues.empty())
            {
                return nan("");
            }
            else if (xValues.size() == 1)
            {
                return yValues[0];
            }
            else
            {
                for (size_t i = 0; i < xValues.size() - 1; i++)
                {
                    if (x >= xValues[i] && x <= xValues[i + 1])
                    {
                        return interpolate(x, xValues[i], yValues[i], xValues[i + 1], yValues[i + 1], extrapolate, interpolationType);
                    }
                    else if (x >= xValues[i + 1] && x <= xValues[i])
                    {
                        return interpolate(x, xValues[i], yValues[i], xValues[i + 1], yValues[i + 1], extrapolate, interpolationType);
                    }
                }

                if (xValues[0] < xValues[xValues.size() - 1])
                {
                    if (x < xValues[0])
                    {
                        return interpolate(x, xValues[0], yValues[0], xValues[1], yValues[1], extrapolate, interpolationType);
                    }
                    else if (x > xValues[xValues.size() - 1])
                    {
                        return interpolate(x, xValues[xValues.size() - 2], yValues[xValues.size() - 2], xValues[xValues.size() - 1], yValues[xValues.size() - 1], extrapolate, interpolationType);
                    }
                }
                else
                {
                    if (x > xValues[0])
                    {
                        return interpolate(x, xValues[0], yValues[0], xValues[1], yValues[1], extrapolate, interpolationType);
                    }
                    else if (x < xValues[xValues.size() - 1])
                    {
                        return interpolate(x, xValues[xValues.size() - 2], yValues[xValues.size() - 2], xValues[xValues.size() - 1], yValues[xValues.size() - 1], extrapolate, interpolationType);
                    }
                }

                return std::nan("");
            }
        }

        std::vector<std::vector<double>> NumericSupport::getFullFactorialCombination(std::vector<std::vector<double>>& sourceLists)
        {
            std::vector<std::vector<double>> combinations = { std::vector<double>() };

            for (int i = sourceLists.size() - 1; i >= 0; i--)
            {
                std::vector<std::vector<double>> previousLists;
                previousLists.insert(previousLists.end(), combinations.begin(), combinations.end());
                combinations.clear();

                for (auto value : sourceLists[i])
                {
                    for (auto previousList : previousLists)
                    {
                        std::vector<double> newList;
                        newList.insert(newList.end(), previousList.begin(), previousList.end());
                        newList.push_back(value);

                        combinations.push_back(newList);
                    }
                }
            }

            std::vector<std::vector<double>> reversedCombinations;

            for (auto list : combinations)
            {
                std::reverse(list.begin(), list.end());
                reversedCombinations.push_back(list);
            }

            return reversedCombinations;
        }

        double NumericSupport::getMean(std::vector<double>& values)
        {
            if (values.empty())
            {
                return nan("");
            }
            else
            {
                double sum = 0;
                for (double x : values)
                {
                    sum += x;
                }

                return sum / values.size();
            }
        }

        double NumericSupport::getWeightedMean(std::vector<double>& values, std::vector<double>& weights)
        {
            if (values.size() != weights.size())
            {
                throw Reliability::probLibException("Values and weights should have same size");
            }

            if (values.empty())
            {
                return nan("");
            }
            else
            {
                double sum = 0;
                double sumWeights = 0;

                for (size_t i = 0; i < values.size(); i++ )
                {
                    sum += values[i] * weights[i];
                    sumWeights += weights[i];
                }

                return sum / sumWeights;
            }
        }

        double NumericSupport::getStandardDeviation(double mean, std::vector<double>& values)
        {
            if (values.empty())
            {
                return nan("");
            }
            else if (values.size() == 1)
            {
                return 0;
            }
            else
            {
                double sum = 0;
                for (double x : values)
                {
                    sum += (x - mean) * (x - mean);
                }

                return sqrt(sum / (values.size() - 1));
            }
        }

        double NumericSupport::getStandardDeviation(std::vector<double>& values)
        {
            double mean = getMean(values);

            return getStandardDeviation(mean, values);
        }

        std::vector<double> NumericSupport::select(std::vector<double>& values, std::function<double(double)> function)
        {
            std::vector<double> result(values.size());

            for (size_t i = 0; i < values.size(); i++)
            {
                result[i] = function(values[i]);
            }

            return result;
        }

        std::vector<double> NumericSupport::zip(std::vector<double>& values1, std::vector<double>& values2, std::function<double(double, double)> function)
        {
            if (values1.size() != values2.size())
            {
                throw Reliability::probLibException("Expected vectors of the same size");
            }

            std::vector<double> result(values1.size());

            for (size_t i = 0; i < values1.size(); i++)
            {
                result[i] = function(values1[i], values2[i]);
            }

            return result;
        }

        double NumericSupport::sum(std::vector<double>& values)
        {
            double sum = 0;

            for (size_t i = 0; i < values.size(); i++)
            {
                sum += values[i];
            }

            return sum;
        }

        double NumericSupport::sum(std::vector<double>& values, std::function<double(double)> function)
        {
            double sum = 0;

            for (size_t i = 0; i < values.size(); i++)
            {
                sum += function(values[i]);
            }

            return sum;
        }

        /// <summary>
        /// Indicates whether a numeric value is a valid value
        /// </summary>
        /// <param name="x"></param>
        /// <returns></returns>
        bool NumericSupport::isValidValue(double x)
        {
            return !std::isnan(x) && !std::isinf(x);
        }

        /// <summary>
        /// Estimates the minimum value for which a function gives a valid result
        /// </summary>
        /// <param name="function"></param>
        /// <returns></returns>
        double NumericSupport::getMinValidValue(std::function<double(double)> function)
        {
            const double minValue = 1E-9;
            const double maxValue = 1E9;

            double minStart = minValue;
            while (minStart < maxValue && !NumericSupport::isValidValue(function(minStart)))
            {
                minStart *= 10;
            }

            return minStart;
        }

        /// <summary>
        /// Estimates the maximum value for which a function gives a valid result
        /// </summary>
        /// <param name="function"></param>
        /// <returns></returns>
        double NumericSupport::getMaxValidValue(std::function<double(double)> function)
        {
            const double minValue = 1E-9;
            const double maxValue = 1E9;

            double maxStart = maxValue;
            while (maxStart > minValue && !NumericSupport::isValidValue(function(maxStart)))
            {
                maxStart /= 10;
            }

            return maxStart;
        }

        /// <summary>
        /// Gets the value whose absolute is value is max
        /// </summary>
        /// <param name="val1"></param>
        /// <param name="val2"></param>
        /// <returns> max value </returns>
        double NumericSupport::getMaxAbs(const double val1, const double val2)
        {
            if (std::fabs(val1) >= std::fabs(val2))
            {
                return val1;
            }
            else
            {
                return val2;
            }
        }

        std::vector<double> NumericSupport::LinearSpaced(const int length, const double start, const double stop)
        {
            if (length < 0)
            {
                throw Reliability::probLibException("length in LinearSpaced < 0");
            }

            switch (length)
            {
            case 0:
                return std::vector<double> {};
            case 1:
                return std::vector<double> { stop };
            default:
            {
                double num = (stop - start) / (double)(length - 1);
                auto array = std::vector<double>();
                for (int i = 0; i < length; i++)
                {
                    array.push_back(start + (double)i * num);
                }

                array[length - 1] = stop;
                return array;
            }
            }
        }



    }
}







