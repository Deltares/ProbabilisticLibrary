#include "StochastPoint.h"

std::shared_ptr<Sample> StochastPoint:: getSample()
{
	std::shared_ptr<Sample> sample = std::make_shared<Sample>(Alphas.size());

	for (int i = 0; i < Alphas.size(); i++)
	{
		sample->Values[i] = this->Alphas[i]->U;
	}

	return sample;
}

