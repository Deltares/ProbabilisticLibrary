#include <vector>
#include "RandomSampleGenerator.h"

#include "../Statistics/StandardNormal.h"
#include "../Math/Random.h"

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
	for (int i = 0; i < this->Settings->StochastSet->getStochastCount(); i++)
	{
		randomValues.push_back(Deltares::Numeric::Random::next());
	}

	Sample* sample = new Sample(this->Settings->StochastSet->getVaryingStochastCount());

	for (int i = 0; i < this->Settings->StochastSet->getVaryingStochastCount(); i++)
	{
		double x = randomValues[this->Settings->StochastSet->VaryingStochastSettings[i]->StochastIndex];
		sample->Values[i] = Deltares::Statistics::StandardNormal::getUFromQ(x);
	}

	return sample;
}
