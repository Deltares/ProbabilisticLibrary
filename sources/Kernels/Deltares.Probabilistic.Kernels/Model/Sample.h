#pragma once

#include <cmath>
#include <limits>

class Sample
{
private:

public:
	Sample() {};

	Sample(int size)
	{
		Values = new double[size];

		for (int i = 0; i < size; i++) {
			Values[i] = 0;
		}
	}

	double* Values;

	int IterationIndex = -1;

	double Weight = 1;

	bool AllowProxy = true;

	double Z = std::numeric_limits<double>::quiet_NaN();
};

