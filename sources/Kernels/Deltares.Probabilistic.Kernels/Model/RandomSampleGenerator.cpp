#include <vector>
#include "RandomSampleGenerator.h"

#include "../Statistics/StandardNormal.h"
#include "../Math/Random.h"

using namespace System;

void RandomSampleGenerator::initialize()
{
	Deltares::Numeric::Random::initialize(this->Settings->RandomGeneratorType, this->Settings->IsRepeatableRandom, this->Settings->Seed, this->Settings->SeedB);
}

void RandomSampleGenerator::restart()
{
	Deltares::Numeric::Random::restart();
}

Sample* RandomSampleGenerator::getRandomSample()
{
	auto randomValues = std::vector<double>();
	for (int i = 0; i < this->Settings->StochastSet->StochastCount; i++)
	{
		randomValues.push_back(Deltares::Numeric::Random::next());
	}

	Sample* sample = new Sample(this->Settings->StochastSet->VaryingStochastCount);

	for (int i = 0; i < this->Settings->StochastSet->VaryingStochastCount; i++)
	{
		double x = randomValues[this->Settings->StochastSet->VaryingStochastSettings[i]->StochastIndex];
		sample->Values[i] = StandardNormal::getUFromQ(x);
	}

	return sample;
}
