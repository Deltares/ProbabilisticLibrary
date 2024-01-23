#pragma once

#include "../../Deltares.Probabilistic.Kernels/Statistics/StandardNormal.h"
#include "../../Deltares.Probabilistic.Kernels/Statistics/StandardNormal.cpp"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class StandardNormalWrapper
			{
			public:
				static double GetUFromP(double p) { return StandardNormal::getUFromP(p); }
				static double GetUFromQ(double q) { return StandardNormal::getUFromQ(q); }
				static double GetPFromU(double u) { return StandardNormal::getPFromU(u); }
				static double GetQFromU(double u) { return StandardNormal::getQFromU(u); }
				static double GetRFromP(double p) { return StandardNormal::getRFromP(p); }
				static double GetPFromR(double r) { return StandardNormal::getPFromR(r); }
			};
		}
	}
}

