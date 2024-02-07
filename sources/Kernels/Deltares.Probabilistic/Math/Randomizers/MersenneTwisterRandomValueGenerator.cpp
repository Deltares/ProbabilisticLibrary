#include "MersenneTwisterRandomValueGenerator.h"

#include <cstdlib>
#include <ctime>

#include "../NumericSupport.h"

void MersenneTwisterRandomValueGenerator::initialize(bool repeatable, int seed, int seedB)
{
	if (repeatable) 
	{
		srand(seed);
	}
	else
	{
		srand(time(nullptr));
	}
}

double MersenneTwisterRandomValueGenerator::next()
{
	return Deltares::Numeric::NumericSupport::Divide(rand(), RAND_MAX);
};


