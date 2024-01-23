#pragma once
#include <vector>

enum DoubleType {Positive, Negative, Zero, NaN};

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
    static double* getCopy(double* values, int count);
    static double* getCopy(const std::vector<double> & values);
    static double* getArray(double initialValue, int count);
    static DoubleType getDoubleType(double value);
    static double getSign(DoubleType value);
    static void swap(double& x, double& y);
};

