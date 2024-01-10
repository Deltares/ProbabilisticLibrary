#include "ModifiedKnuthSubtractiveRandomValueGenerator.h"

void ModifiedKnuthSubtractiveRandomValueGenerator::initialize(int seed, int seedB)
{
	ModifiedKnuthSubtractiveRandomValueGenerator::initializeDelegate(seed);
}

double ModifiedKnuthSubtractiveRandomValueGenerator::next()
{
	return ModifiedKnuthSubtractiveRandomValueGenerator::nextDelegate();
};

