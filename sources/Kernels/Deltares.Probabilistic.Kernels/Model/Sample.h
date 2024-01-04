#pragma once

#include <cmath>
#include <limits>

#include "../Math/NumericSupport.h"

class Sample
{
private:
	int size = 0;

public:
	Sample(int size)
	{
		this->size = size;
		Values = new double[size];

		for (int i = 0; i < size; i++) 
		{
			Values[i] = 0;
		}
	}

	Sample(double* values, int size)
	{
		this->size = size;
		Values = new double[size];

		for (int i = 0; i < size; i++) 
		{
			Values[i] = 0;
		}
	}

	double* Values;
	int IterationIndex = -1;
	int ScenarioIndex = -1;
	double Weight = 1;
	bool AllowProxy = true;
	double Z = std::numeric_limits<double>::quiet_NaN();

	int getSize() const
	{
		return size;
	}

	double getBeta()
	{
		return NumericSupport::GetLength(Values, size);
	}

	void setBeta(double beta)
	{
		double value = sqrt(beta * beta / size) * NumericSupport::GetSign(beta);

		for (int i = 0; i < size; i++)
		{
			Values[i] = value;
		}
	}
};

