#include <vector>
#include "RandomSampleGenerator.h"

#include "../Statistics/StandardNormal.h"
#include "../Math/Random.h"

namespace Deltares
{
	namespace Models
	{
		void RandomSampleGenerator::initialize()
		{
			Deltares::Numeric::Random::initialize(this->Settings->RandomGeneratorType, this->Settings->IsRepeatableRandom, this->Settings->Seed, this->Settings->SeedB);
		}

		void RandomSampleGenerator::restart()
		{
			Deltares::Numeric::Random::restart();
		}

		std::shared_ptr<Sample> RandomSampleGenerator::getRandomSample()
		{
			std::vector<double> randomValues;

			const int size = this->Settings->SkipUnvaryingParameters ? this->Settings->StochastSet->getStochastCount() : this->Settings->StochastSet->getVaryingStochastCount();
			for (int i = 0; i < size; i++)
			{
				randomValues.push_back(Deltares::Numeric::Random::next());
			}

			std::shared_ptr<Sample> sample = std::make_shared<Sample>(this->Settings->StochastSet->getVaryingStochastCount());

			for (int i = 0; i < this->Settings->StochastSet->getVaryingStochastCount(); i++)
			{
				const int variableIndex = this->Settings->SkipUnvaryingParameters ? this->Settings->StochastSet->VaryingStochastSettings[i]->StochastIndex : i;
				const double x = randomValues[variableIndex];
				sample->Values[i] = Deltares::Statistics::StandardNormal::getUFromQ(x);
			}

			return sample;
		}
	}
}
