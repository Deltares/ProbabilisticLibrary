#pragma once

#include <cmath>
#include <cstdint>

namespace Deltares
{
	namespace Models
	{
		class Evaluation
		{
		public:
			double* X;
			int SizeX;

			double* R;
			int SizeR;

			double Z = nan("");
			double Weight = 1;
			int Iteration = -1;

			intptr_t Tag = 0;
		};
	}
}

