#pragma once
#include <functional>
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
            static double GetSquaredSum(const std::vector<double>& values);
            static double GetLength(const std::vector<double>& values);
            static std::vector<double> GetSphericalCoordinates(const std::vector<double>& cartesianCoordinates);
            static std::vector<double> GetCartesianCoordinates(const std::vector<double>& sphericalCoordinates);
            static double Divide(int counter, int denominator);
            static double round(double value, int decimals);
            static bool areEqual(double x1, double x2, double tolerance);
            static bool isLess(double x1, double x2, double tolerance);
            static bool isGreater(double x1, double x2, double tolerance);
            static CmpResult compareDouble(const double x1, const double x2, const double tol = 1e-14);
            static double* getCopy(double* values, int count);
            static double* getCopy(const std::vector<double>& values);
            static double* getArray(double initialValue, int count);
            static DoubleType getDoubleType(double value);
            static double getSign(DoubleType value);
            static double getFraction(double value, double fraction);
            static bool isValidValue(double x);
            static double getMinimum(std::vector<double>& values);
            static double getMaximum(std::vector<double>& values);
            static double getMinValidValue(std::function<double(double)> function);
            static double getMaxValidValue(std::function<double(double)> function);
            static double interpolate(double x, double minX, double minY, double maxX, double maxY, bool extrapolate = false, InterpolationType interpolationType = Linear);
            static double interpolate(double x, const std::vector<double>& xValues, const std::vector<double>& yValues, bool extrapolate = false, InterpolationType interpolationType = Linear);
            static std::vector<std::vector<double>> getFullFactorialCombination(std::vector<std::vector<double>>& sourceLists);
            static double getMean(std::vector<double>& values);
            static double getStandardDeviation(double mean, std::vector<double>& values);
            static double getStandardDeviation(std::vector<double>& values);
            static std::vector<double> select(std::vector<double>& values, std::function<double(double)> function);
            static double sum(std::vector<double>& values, std::function<double(double)> function);
        };
    }
}

