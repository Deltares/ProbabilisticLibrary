#include "NumericSupport.h"

#include <cmath>
#include <exception>

#include "Constants.h"
#include "../Utils/probLibException.h"

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
		throw Deltares::ProbLibCore::probLibException("double type not supported");
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

	for (size_t i = 0; i < count; i++)
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

double NumericSupport::interpolate(double x, double minX, double minY, double maxX, double maxY, bool extrapolate, InterpolationType interpolationType)
{
	if (minX > maxX ^ interpolationType == InterpolationType::Harmonic)
	{
		double temp = minX;
		minX = maxX;
		maxX = temp;

		temp = minY;
		minY = maxY;
		maxY = temp;
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
		x = 1 / x;
		minX = 1 / minX;
		maxX = 1 / maxX;
		break;
	default:
		throw std::exception("interpolation type not supported");
	}

	if (x < minX && !extrapolate)
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
		return xFraction * maxY + (1 - xFraction) * minY;
	}
}

double NumericSupport::interpolate(double x, std::vector<double>& xValues, std::vector<double>& yValues, bool extrapolate, InterpolationType interpolationType)
{
	if (xValues.size() != yValues.size())
	{
		throw std::exception("XValues and YValues not of the same length");
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
		for (int i = 0; i < xValues.size() - 1; i++)
		{
			if (x >= xValues[i] && x <= xValues[i + 1])
			{
				return interpolate(x, xValues[i], yValues[i], xValues[i + 1], yValues[i + 1], extrapolate, interpolationType);
			}
			else if (x >= xValues[i + 1] && x <= xValues[i])
			{
				return interpolate(x, xValues[i],  yValues[i], xValues[i + 1],yValues[i + 1], extrapolate, interpolationType);
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
				return interpolate(x, xValues[xValues.size() - 2], yValues[xValues.size() - 2], xValues[xValues.size() - 1],  yValues[xValues.size() - 1], extrapolate, interpolationType);
			}
		}

		return nan("");
	}
}






