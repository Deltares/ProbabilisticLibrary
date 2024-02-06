#pragma once

#include "StochastProperties.h"

namespace Deltares
{
	namespace Statistics
	{
		class VariableStochastValue
		{
		public:
			double X;
			std::shared_ptr<StochastProperties> Stochast = std::make_shared<StochastProperties>();
		};
	}
}

