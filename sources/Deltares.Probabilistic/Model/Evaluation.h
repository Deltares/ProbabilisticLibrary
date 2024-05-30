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
			double Z = nan("");
			double Weight = 1;
			int Iteration = -1;

            bool isOwnerOfTag = true;
			intptr_t Tag = 0;
		};
	}
}

