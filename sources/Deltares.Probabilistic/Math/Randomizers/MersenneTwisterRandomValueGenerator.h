#pragma once
#include <random>
#include "RandomValueGenerator.h"

namespace Deltares
{
	namespace Numeric
	{
		class MersenneTwisterRandomValueGenerator : public RandomValueGenerator
		{
		public:
			void initialize(bool repeatable, int seed, int seedB) override;
			double next() override;
		private:
            std::mt19937_64 generator;
		};
	}
}

