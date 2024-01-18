#include "StochastPoint.h"

Sample* StochastPoint:: getSample()
{
	double* uValues = new double[Alphas.size()];

	for (int i = 0; i < Alphas.size(); i++)
	{
		uValues[i] = this->Alphas[i]->U;
	}

	return new Sample(uValues, Alphas.size());
}

