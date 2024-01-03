#pragma once

#include <vector>

#include "Sample.h"
#include "../Statistics/Stochast.h"


class UConverter
{
private:
	std::vector<Stochast> stochasts;
	std::vector<Stochast> varyingStochasts;
	std::vector<int> varyingStochastIndex;

	double* getExpandedUValues(double* values);

public:

	UConverter()
	{
	}

	UConverter(std::vector<Stochast> stochasts)
	{
		this->stochasts.clear();

		for (int i = 0; i < stochasts.size(); i++)
		{
			this->stochasts.push_back(stochasts[i]);
		}
	}

	void initializeForRun();

	double* getXValues(Sample sample);
};

