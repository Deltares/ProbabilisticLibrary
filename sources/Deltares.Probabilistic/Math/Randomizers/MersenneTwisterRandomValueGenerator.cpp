#include "MersenneTwisterRandomValueGenerator.h"

#include <cstdlib>
#include <ctime>

#include "../NumericSupport.h"

namespace Deltares
{
	namespace Numeric
	{
		void MersenneTwisterRandomValueGenerator::initialize(bool repeatable, int seed, int seedB)
		{
			if (repeatable)
			{
				generator.seed(seed);
			}
			else
			{
				generator.seed(time(nullptr));
			}
		}

		double MersenneTwisterRandomValueGenerator::next()
		{
            std::uniform_real_distribution<double> distribution(0.0, 1.0);
            double ri = distribution(generator);
            return ri;
		};
	}
}


