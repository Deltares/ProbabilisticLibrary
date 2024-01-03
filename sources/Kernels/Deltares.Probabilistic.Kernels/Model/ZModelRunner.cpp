#include "pch.h"
#include "ZModelRunner.h"

double ZModelRunner::getZValue(Sample sample)
{
	double* xValues = this->uConverter.getXValues(sample);

	sample.Z = this->zModel.getZValue(xValues);

	delete[] xValues;

	return sample.Z;
	
}

double* ZModelRunner::getZValues(std::vector<Sample> samples)
{
	double* zValues = new double[samples.size()];

	for (int i = 0; i < samples.size(); i++)
	{
		zValues[i] = getZValue(samples[i]);
	}

	return zValues;
}

