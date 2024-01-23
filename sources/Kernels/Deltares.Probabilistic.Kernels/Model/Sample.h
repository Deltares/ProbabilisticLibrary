#pragma once

#include <cmath>
#include <vector>
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
		for (size_t i = 0; i < size; i++)
		{
			Values.push_back(0.0);
		}
	}

	Sample(double* values, int size)
	{
		this->size = size;
		for (size_t i = 0; i < size; i++)
		{
			this->Values.push_back(values[i]);
		}
	}

	Sample(std::vector<double> & values, int size)
	{
		this->size = size;
		this->Values = values;
	}

	~Sample()
	{
	}

	std::vector<double> Values;

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

