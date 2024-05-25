#include <vector>
#include "RandomSampleGenerator.h"

#include "SampleProvider.h"
#include "../Statistics/StandardNormal.h"
#include "../Math/Random.h"

namespace Deltares
{
	namespace Models
	{
		void RandomSampleGenerator::initialize()
		{
			Deltares::Numeric::Random::initialize(this->Settings->RandomGeneratorType, this->Settings->IsRepeatableRandom, this->Settings->Seed, this->Settings->SeedB);

            if (sampleProvider == nullptr)
            {
                sampleProvider = std::make_shared<SampleProvider>(this->Settings->StochastSet, false);
            }
		}

		void RandomSampleGenerator::restart()
		{
			Deltares::Numeric::Random::restart();

		    if (sampleProvider != nullptr)
            {
                sampleProvider->reset();
            }
		}

		std::shared_ptr<Sample> RandomSampleGenerator::getRandomSample()
		{
			std::vector<double> randomValues;

			const int size = this->Settings->SkipUnvaryingParameters ? this->Settings->StochastSet->getStochastCount() : this->Settings->StochastSet->getVaryingStochastCount();
			for (int i = 0; i < size; i++)
			{
				randomValues.push_back(Deltares::Numeric::Random::next());
			}

			std::shared_ptr<Sample> sample = sampleProvider->getSample();

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
