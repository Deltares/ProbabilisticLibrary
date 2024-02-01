#pragma once

#include "DiscreteDistribution.h"

namespace Deltares
{
	namespace Statistics
	{
		class QualitativeDistribution : public DiscreteDistribution
		{
			bool isQualitative() override { return true; }
		};
	}
}

