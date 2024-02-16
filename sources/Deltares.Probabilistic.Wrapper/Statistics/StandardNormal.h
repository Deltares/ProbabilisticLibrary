#pragma once

#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			public ref class StandardNormal
			{
			public:
				static double GetUFromP(double p) { return Statistics::StandardNormal::getUFromP(p); }
				static double GetUFromQ(double q) { return Statistics::StandardNormal::getUFromQ(q); }
				static double GetPFromU(double u) { return Statistics::StandardNormal::getPFromU(u); }
				static double GetQFromU(double u) { return Statistics::StandardNormal::getQFromU(u); }
				static double GetRFromP(double p) { return Statistics::StandardNormal::getRFromP(p); }
				static double GetPFromR(double r) { return Statistics::StandardNormal::getPFromR(r); }

				static property double UMax
				{
					double get() { return Statistics::StandardNormal::UMax; }
				}

				static property double BetaMax
				{
					double get() { return Statistics::StandardNormal::BetaMax; }
				}

				static bool IsBetaMin(double beta)
				{
					return beta <= -BetaMax;
				}

				static bool IsBetaMax(double beta)
				{
					return beta >= BetaMax;
				}
			};
		}
	}
}

