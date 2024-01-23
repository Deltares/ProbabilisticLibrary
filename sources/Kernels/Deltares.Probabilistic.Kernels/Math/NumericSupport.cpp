#include "NumericSupport.h"

#include <cmath>
#include <exception>

#include "Constants.h"


double NumericSupport::GetSign(double value)
{
	return value > 0.0 ? 1 : -1;
}

double NumericSupport::GetSquaredSum(const std::vector<double> & values)
{
	double sum = 0;
	for (int i = 0; i < values.size(); i++)
	{
		sum += values[i] * values[i];
	}

	return sum;
}

double NumericSupport::GetLength(const std::vector<double> & values)
{
	return sqrt(GetSquaredSum(values));
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

double* NumericSupport::getCopy(const std::vector<double> & values)
{
	auto count = values.size();
	double* newValues = new double[count];

	for (int i = 0; i < count; i++)
	{
		newValues[i] = values[i];
	}

	return newValues;
}

void NumericSupport::swap(double& x, double& y)
{
	double swapped = x;
	x = y;
	y = swapped;
}



double NumericSupport::Divide(int counter, int denominator)
{
	return static_cast<double>(counter) / static_cast<double>(denominator);
}

bool NumericSupport::areEqual(double x1, double x2, double tolerance)
{
	if (isnan(x1) && isnan(x2))
	{
		return true;
	}
	else
	{
		return abs(x1 - x2) < tolerance;
	}
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
		//C# TO C++ CONVERTER TASK: There is no C++ equivalent to 'ToString':
		throw new std::exception("double type not supported");
	}
}


/// <summary>
/// Converts cartesian coordinates to spherical coordinates
/// </summary>
/// <param name="cartesianCoordinates"></param>
/// <param name="count"></param>
/// <returns></returns>
/// <see cref="https://en.wikipedia.org/wiki/N-sphere#Spherical_coordinates"/>
std::vector<double> NumericSupport::GetSphericalCoordinates(const std::vector<double> & cartesianCoordinates)
{
	auto count = cartesianCoordinates.size();
	auto coordinates = std::vector<double>(count);

	coordinates[0] = GetSquaredSum(cartesianCoordinates);

	for (int i = 1; i < count; i++)
	{
		if (cartesianCoordinates[i - 1] != 0)
		{
			double sum = 0;
			for (int j = i - 1; j < count; j++)
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
/// <param name="count"></param>
/// <returns></returns>
/// <see cref="https://en.wikipedia.org/wiki/N-sphere#Spherical_coordinates"/>
std::vector<double> NumericSupport::GetCartesianCoordinates(const std::vector<double> & sphericalCoordinates)
{
	auto count = sphericalCoordinates.size();
	auto coordinates = std::vector<double>(count);

	for (int i = 0; i < count; i++)
	{
		coordinates[i] = sphericalCoordinates[0];
		for (int j = 0; j < i; j++)
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




