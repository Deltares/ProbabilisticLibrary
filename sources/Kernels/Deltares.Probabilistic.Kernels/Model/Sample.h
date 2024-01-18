#pragma once

#include <cmath>

#include "../Math/NumericSupport.h"

class Sample
{
private:
	int size = 0;
	long tag = 0;

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

	~Sample()
	{
		delete[] Values;
		delete[] XValues;
	}

	double* Values;
	int sizeX = 0;
	double* XValues;

	int IterationIndex = -1;
	int ScenarioIndex = -1;
	double Weight = 1;
	bool AllowProxy = true;
	double Z = nan("");

	int getSize();
	int getSizeX();

	double getBeta();

	void setBeta(double beta);

	intptr_t Tag = 0;
};

