#pragma once

#include <cmath>
#include <cstdint>

#include "../Utils/TagSupport.h"

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
			int Tag = 0;
		};
	}
}

