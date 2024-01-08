#include "NumericSupport.h"

#include <cmath>

const double pi = 3.141592653589793238795028841971;

double NumericSupport::GetSign(double value)
{
    return value > 0.0 ? 1 : -1;
}

double NumericSupport::GetSquaredSum(double* values, int count)
{
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += values[i] * values[i];
    }

    return sum;
}

double NumericSupport::GetLength(double* values, int count)
{
    return sqrt(GetSquaredSum(values, count));
}

double NumericSupport::Divide(int counter, int denominator)
{
    return static_cast<double>(counter) / static_cast<double>(denominator);
}


/// <summary>
/// Converts cartesian coordinates to spherical coordinates
/// </summary>
/// <param name="cartesianCoordinates"></param>
/// <param name="count"></param>
/// <returns></returns>
/// <see cref="https://en.wikipedia.org/wiki/N-sphere#Spherical_coordinates"/>
double* NumericSupport::GetSphericalCoordinates(double* cartesianCoordinates, int count)
{
    double* coordinates = new double[count];

    coordinates[0] = GetSquaredSum(cartesianCoordinates, count);

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
            coordinates[i] = 2 * pi - coordinates[i];
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
double* NumericSupport::GetCartesianCoordinates(double* sphericalCoordinates, int count)
{
    double* coordinates = new double[count];

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




