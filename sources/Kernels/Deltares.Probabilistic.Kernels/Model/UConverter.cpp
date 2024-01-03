#include "pch.h"
#include "UConverter.h"
#include "../Statistics/Stochast.h"

void UConverter::initializeForRun()
{
	this->varyingStochasts.clear();
	this->varyingStochastIndex.clear();

	for (int i = 0; i < this->stochasts.size(); i++)
	{
		if (this->stochasts[i].isVarying()) 
		{
			this->varyingStochastIndex.push_back(this->varyingStochastIndex.size());
			this->varyingStochasts.push_back(this->stochasts[i]);
		}
		else
		{
			this->varyingStochastIndex.push_back(-1);
		}
	}
}

double* UConverter::getExpandedUValues(double* values)
{
	double* uValues = new double[this->stochasts.size()];

	for (int i = 0; i < this->stochasts.size(); i++)
	{
		const int varyingIndex = this->varyingStochastIndex[i];
		if (varyingIndex >= 0)
		{
			uValues[i] = values[varyingIndex];
		}
		else
		{
			uValues[i] = 0;
		}
	}

	return uValues;
}


double* UConverter::getXValues(Sample sample)
{
	double* expandedUValues = getExpandedUValues(sample.Values);

	double* xValues = new double[this->stochasts.size()];

	for (int i = 0; i < this->stochasts.size(); i++)
	{
		xValues[i] = this->stochasts[i].getXFromU(expandedUValues[i]);
	}

	delete[] expandedUValues;

	return xValues;
}

