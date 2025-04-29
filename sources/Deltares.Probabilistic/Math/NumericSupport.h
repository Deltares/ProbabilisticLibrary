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
#include <functional>
#include <string>
#include <vector>

namespace Deltares
{
    namespace Numeric
    {
        enum DoubleType { Positive, Negative, Zero, NaN };
        enum InterpolationType { Linear, Logarithmic, Harmonic };
        enum CmpResult { Equal, Greater, Smaller };

        class NumericSupport
        {
        private:
        public:
            static double GetSign(double value);
            static double GetPrevailingSign(std::vector<double> values);
            static double GetSquaredSum(const std::vector<double>& values);
            static double GetLength(const std::vector<double>& values);
            static double getDistance(const std::vector<double>& values1, const std::vector<double>& values2);
            static double getDistance2(const std::vector<double>& values1, const std::vector<double>& values2);
            static std::vector<double> GetSphericalCoordinates(const std::vector<double>& cartesianCoordinates);
            static std::vector<double> GetCartesianCoordinates(const std::vector<double>& sphericalCoordinates);
            static double Divide(int counter, int denominator);
            static double round(double value, int decimals);
            static bool areEqual(double x1, double x2, double tolerance);
            static bool isLess(double x1, double x2, double tolerance);
            static bool isGreater(double x1, double x2, double tolerance);
            static bool isBetween(double xLow, double x, double xHigh, double tolerance);
            static CmpResult compareDouble(const double x1, const double x2, const double tol = 1e-14);
            static bool doublesAreEqual(const std::vector<double>& p1, const std::vector<double>& p2, const double tol = 1e-14);
            static double* getCopy(double* values, int count);
            static double* getCopy(const std::vector<double>& values);
            static double* getArray(double initialValue, int count);
            static DoubleType getDoubleType(double value);
            static double getSign(DoubleType value);
            static double getFraction(double value, double fraction);
            static bool isValidValue(double x);
            static double getMinimum(const std::vector<double>& values);
            static double getMaximum(const std::vector<double>& values);
            static size_t getLocationMinimum(const std::vector<double>& values);
            static size_t getLocationMaximum(const std::vector<double>& values);
            static double getMinValidValue(std::function<double(double)> function);
            static double getMaxValidValue(std::function<double(double)> function);
            static double interpolate(double x, double minX, double minY, double maxX, double maxY, bool extrapolate = false, InterpolationType interpolationType = Linear);
            static double interpolate(double x, const std::vector<double>& xValues, const std::vector<double>& yValues, bool extrapolate = false, InterpolationType interpolationType = Linear);
            static double limit(const double x, double minVal, double maxVal);

            /**
             * \brief Gets the full factorial combination of a number of lists
             * \param sourceLists Lists to be combined
             * \return Full factorial combination
             * \remark {1,2,3} + {10, 11} becomes {1,10}, {1,11}, {2,10}, {2,11}, {3,10}, {3, 11}
             */
            static std::vector<std::vector<double>> getFullFactorialCombination(std::vector<std::vector<double>>& sourceLists);
            static double getMean(std::vector<double>& values);
            static double getStandardDeviation(double mean, std::vector<double>& values);
            static double getWeightedMean(std::vector<double>& values, std::vector<double>& weights);
            static double getStandardDeviation(std::vector<double>& values);
            static std::vector<double> select(std::vector<double>& values, std::function<double(double)> function);
            static std::vector<double> zip(std::vector<double>& values1, std::vector<double>& values2, std::function<double(double, double)> function);
            static double sum(std::vector<double>& values);
            static double sum(std::vector<double>& values, std::function<double(double)> function);
            static double getMaxAbs(const double val1, const double val2);
            static std::vector<double> LinearSpaced(const int length, const double start, const double stop);
            static std::string ConvertToString(std::vector<double>& values, const std::string& separator);
        };
    }
}

