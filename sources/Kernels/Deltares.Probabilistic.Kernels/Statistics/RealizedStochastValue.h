#pragma once
#include <vector>

#include "StochastProperties.h"

namespace Deltares
{
	namespace Statistics
	{
		class RealizedStochastValue
		{
		public:
			double X;
			std::shared_ptr<StochastProperties> Stochast;
		};
	}
}

