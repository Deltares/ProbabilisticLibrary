#include "MersenneTwisterRandomValueGenerator.h"

#include <cstdlib>

#include "../NumericSupport.h"

void MersenneTwisterRandomValueGenerator::initialize(int seed, int seedB)
{
	srand(seed);
}

double MersenneTwisterRandomValueGenerator::next()
{
	return NumericSupport::Divide(rand(), RAND_MAX);
};


