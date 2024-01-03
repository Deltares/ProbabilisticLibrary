#pragma once
#include "Sample.h"
#include <vector>

#include "UConverter.h"
#include "ZModel.h"

class ZModelRunner
{
private:
	ZModel zModel;
	UConverter uConverter;

public:
	ZModelRunner(ZModel zModel, UConverter uConverter)
	{
		this->zModel = zModel;
		this->uConverter = uConverter;
	}

	double getZValue(Sample sample);
	double* getZValues(std::vector<Sample> samples);
};

