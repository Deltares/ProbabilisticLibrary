#pragma once
#include "Randomizers/RandomValueGenerator.h"

namespace Deltares
{
	namespace Numeric
	{
		enum RandomValueGeneratorType { MersenneTwister, GeorgeMarsaglia, ModifiedKnuthSubtractive };

		class Random
		{
		private:
			inline static int seed = 0;
			inline static int seedB = 0;
			inline static RandomValueGeneratorType generatorType = RandomValueGeneratorType::MersenneTwister;
			inline static RandomValueGenerator* randomValueGenerator = nullptr;

		public:
			static void initialize(RandomValueGeneratorType generatorType, int seed, int seedB = 0);
			static double next();
			static void restart();
		};
	}
}

