#include "CorrelationMatrix.h"

double* CorrelationMatrix::Cholesky(double* uValues, int count)
{
	double* correlatedValues = new double[count];

	for (int i = 0; i < count; i++)
	{
		correlatedValues[i] = uValues[i];
	}

	return correlatedValues;
}

