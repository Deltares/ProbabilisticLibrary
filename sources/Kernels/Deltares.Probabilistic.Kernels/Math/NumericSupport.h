#pragma once

enum DoubleType {Positive, Negative, Zero, NaN};


class NumericSupport
{
private:
public:
	static double GetSign(double value);
	static double GetSquaredSum(double* values, int count);
	static double GetLength(double* values, int count);
	static double* GetSphericalCoordinates(double* cartesianCoordinates, int count);
	static double* GetCartesianCoordinates(double* sphericalCoordinates, int count);
	static double Divide(int counter, int denominator);
	static double round(double value, int decimals);
	static bool areEqual(double x1, double x2, double tolerance);
	static double* getCopy(double* values, int count);
	static double* getArray(double initialValue, int count);
	static DoubleType getDoubleType(double value);
	static double getSign(DoubleType value);
	static void swap(double& x, double& y);
};




