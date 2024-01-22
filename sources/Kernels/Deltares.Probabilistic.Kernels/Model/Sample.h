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
		this->Values = NumericSupport::getArray(0, size);  
	}

	Sample(double* values, int size)
	{
		this->size = size;
		this->Values = NumericSupport::getCopy(values, size);
	}

	~Sample()
	{
		delete[] Values;
	}

	double* Values = new double[0];

	int IterationIndex = -1;
	int ScenarioIndex = -1;
	double Weight = 1;
	bool AllowProxy = true;
	double Z = nan("");

	int getSize();
	int getSizeX();

	double getBeta();
	void setBeta(double beta);
	Sample* clone();
	Sample* normalize(double beta = 1);
	Sample* multiply(double factor);

	intptr_t Tag = 0;
};

