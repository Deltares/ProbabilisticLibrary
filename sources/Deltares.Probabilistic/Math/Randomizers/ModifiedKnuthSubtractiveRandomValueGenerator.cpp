#include "ModifiedKnuthSubtractiveRandomValueGenerator.h"

namespace Deltares
{
	namespace Numeric
	{
        bool ModifiedKnuthSubtractiveRandomValueGenerator::isAvailable()
        {
            return ModifiedKnuthSubtractiveRandomValueGenerator::initializeDelegate != nullptr;
        }

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

