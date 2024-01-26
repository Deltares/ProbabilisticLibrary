#include "StochastPoint.h"

Sample* StochastPoint:: getSample()
{
	Sample* sample = new Sample(Alphas.size());

	for (int i = 0; i < Alphas.size(); i++)
	{
		sample->Values[i] = this->Alphas[i]->U;
	}

	return sample;
}

