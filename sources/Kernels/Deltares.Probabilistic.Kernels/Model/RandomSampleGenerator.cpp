#include "RandomSampleGenerator.h"

#include <ctime>

#include "../Statistics/StandardNormal.h"
#include "../Math/Random.h"

using namespace System;

void RandomSampleGenerator::initialize()
{
	Deltares::Numeric::Random::initialize(this->Settings->RandomGeneratorType, this->Settings->IsRepeatableRandom, this->Settings->Seed, this->Settings->SeedB);

	xSettings = new XSetting * [this->Settings->VaryingStochastCount];

	for (int i = 0; i < this->Settings->VaryingStochastCount; i++)
	{
		xSettings[i] = new XSetting();
		xSettings[i]->xMin = StandardNormal::getPFromU(this->Settings->VaryingStochastSettings[i]->MinValue);
		xSettings[i]->xMax = StandardNormal::getPFromU(this->Settings->VaryingStochastSettings[i]->MaxValue);
		xSettings[i]->isDefault =this->Settings->VaryingStochastSettings[i]->isMinMaxDefault();
	}
}

void RandomSampleGenerator::restart()
{
	Deltares::Numeric::Random::restart();
}

Sample* RandomSampleGenerator::getRandomSample()
{
	double* randomValues = new double[this->Settings->StochastCount];
	for (int i = 0; i < this->Settings->StochastCount; i++)
	{
		randomValues[i] = Deltares::Numeric::Random::next();
	}

	Sample* sample = new Sample(this->Settings->VaryingStochastCount);

	for (int i = 0; i < this->Settings->VaryingStochastCount; i++)
	{
		double x = randomValues[this->Settings->VaryingStochastSettings[i]->StochastIndex];

		if (!xSettings[i]->isDefault)
		{
			x = xSettings[i]->xMin + x * (xSettings[i]->xMax - xSettings[i]->xMin);
		}

		sample->Values[i] = StandardNormal::getUFromQ(x);
	}

	return sample;
}
