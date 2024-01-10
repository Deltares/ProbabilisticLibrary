#include "Random.h"

#include <cstdlib>
#include <exception>

#include "Randomizers/MersenneTwisterRandomValueGenerator.h"
#include "Randomizers/GeorgeMarsagliaRandomValueGenerator.h"
#include "Randomizers/ModifiedKnuthSubtractiveRandomValueGenerator.h"
#include "Randomizers/RandomValueGenerator.h"


namespace Deltares
{
	namespace Numeric
	{
		void Random::initialize(RandomValueGeneratorType generatorType, bool repeatable, int seed, int seedB)
		{
			switch (generatorType)
			{
			case MersenneTwister: Random::randomValueGenerator = new MersenneTwisterRandomValueGenerator(); break;
			case GeorgeMarsaglia: Random::randomValueGenerator = new GeorgeMarsagliaRandomValueGenerator(); break;
			case ModifiedKnuthSubtractive: Random::randomValueGenerator = new ModifiedKnuthSubtractiveRandomValueGenerator(); break;
			default: throw std::exception("Generator type not supported");
			}

			Random::repeatable = repeatable;
			Random::seed = seed;
			Random::seedB = seedB;

			Random::randomValueGenerator->initialize(repeatable, seed, seedB);
		}

		double Random::next()
		{
			return Random::randomValueGenerator->next();
		}

		void Random::restart()
		{
			return Random::randomValueGenerator->initialize(Random::repeatable, Random::seed, Random::seedB);
		}
	}
}




