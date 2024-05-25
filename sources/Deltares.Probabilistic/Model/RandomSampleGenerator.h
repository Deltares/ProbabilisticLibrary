#pragma once
#include "RandomSettings.h"
#include "Sample.h"
#include "SampleProvider.h"

namespace Deltares
{
	namespace Models
	{
		class RandomSampleGenerator
		{
		public:
			std::shared_ptr<RandomSettings> Settings = std::make_shared<RandomSettings>();
            std::shared_ptr<SampleProvider> sampleProvider = nullptr;

			void initialize();
			void restart();
			std::shared_ptr<Sample> getRandomSample();
		};
	}
}
