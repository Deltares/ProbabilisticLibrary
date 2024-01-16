#pragma once
#include "../Statistics/StandardNormal.h"

namespace Deltares
{
	namespace Reliability
	{
		class StochastSettings
		{
		public:
			double MinValue = -StandardNormal::UMax;
			double MaxValue = StandardNormal::UMax;
			double StartValue = 0;

			double VarianceFactor = 1.5;
			bool IsVarianceAllowed = true;

			int Intervals = 200;

			int StochastIndex = 0;

			bool isMinMaxDefault();
		};
	}
}

