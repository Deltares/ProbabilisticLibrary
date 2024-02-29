#include "ModifiedKnuthSubtractiveRandomValueGenerator.h"

namespace Deltares
{
	namespace Numeric
	{
		void ModifiedKnuthSubtractiveRandomValueGenerator::initialize(bool repeatable, int seed, int seedB)
		{
			ModifiedKnuthSubtractiveRandomValueGenerator::initializeDelegate(repeatable, seed);
		}

		double ModifiedKnuthSubtractiveRandomValueGenerator::next()
		{
			return ModifiedKnuthSubtractiveRandomValueGenerator::nextDelegate();
		};
	}
}

