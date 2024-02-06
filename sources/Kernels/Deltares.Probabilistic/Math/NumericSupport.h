#pragma once
#include <vector>

enum DoubleType {Positive, Negative, Zero, NaN};
enum InterpolationType { Linear, Logarithmic, Harmonic };

enum CmpResult { Equal, Greater, Smaller};

class NumericSupport
{
private:
public:
    static double GetSign(double value);
    static double GetSquaredSum(const std::vector<double> & values);
    static double GetLength(const std::vector<double> & values);
    static std::vector<double> GetSphericalCoordinates(const std::vector<double> & cartesianCoordinates);
    static std::vector<double> GetCartesianCoordinates(const std::vector<double> & sphericalCoordinates);
    static double Divide(int counter, int denominator);
    static double round(double value, int decimals);
    static bool areEqual(double x1, double x2, double tolerance);
    static CmpResult compareDouble(const double x1, const double x2, const double tol = 1e-14);
    static double* getCopy(double* values, int count);
    static double* getCopy(const std::vector<double> & values);
    static double* getArray(double initialValue, int count);
    static DoubleType getDoubleType(double value);
    static double getSign(DoubleType value);
    static double interpolate(double x, double minX, double minY, double maxX, double maxY, bool extrapolate = false, InterpolationType interpolationType = Linear);
    static double interpolate(double x, std::vector<double>& xValues, std::vector<double>& yValues, bool extrapolate = false, InterpolationType interpolationType = Linear);
};

