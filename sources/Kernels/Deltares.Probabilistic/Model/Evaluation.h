#pragma once

#include <cmath>
#include <cstdint>
#include <vector>

namespace Deltares
{
	namespace Models
	{
		class Evaluation
		{
		public:
			std::vector<double> X;
			std::vector<double> R;

			double Z = nan("");
			double Weight = 1;
			int Iteration = -1;

			intptr_t Tag = 0;
		};
	}
}

