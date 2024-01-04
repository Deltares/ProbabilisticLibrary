#pragma once

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
};




