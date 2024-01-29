#pragma once

#include <cmath>
#include <vector>
#include <cstdint>
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

	Sample(std::vector<double> & values)
	{
		this->size = values.size();
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

	double getBeta();
	void setBeta(double beta);
	Sample* clone();
	Sample* normalize(double beta = 1);
	Sample* multiply(double factor);
	void correctSmallValues(double tolerance = 1E-10);

	intptr_t Tag = 0;
};

