#pragma once

#include "../Statistics/StandardNormal.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class StandardNormalWrapper
			{
			public:
				static double GetUFromP(double p) { return Statistics::StandardNormal::getUFromP(p); }
				static double GetUFromQ(double q) { return Statistics::StandardNormal::getUFromQ(q); }
				static double GetPFromU(double u) { return Statistics::StandardNormal::getPFromU(u); }
				static double GetQFromU(double u) { return Statistics::StandardNormal::getQFromU(u); }
				static double GetRFromP(double p) { return Statistics::StandardNormal::getRFromP(p); }
				static double GetPFromR(double r) { return Statistics::StandardNormal::getPFromR(r); }
			};
		}
	}
}

