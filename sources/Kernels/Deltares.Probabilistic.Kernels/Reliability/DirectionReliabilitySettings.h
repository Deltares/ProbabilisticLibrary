#pragma once

#include "StochastSettingsSet.h"
#include "../Model/Sample.h"

namespace Deltares
{
	namespace Reliability
	{
		enum ModelVaryingType {Monotone, Varying};

		class DirectionReliabilitySettings
		{
		public:
			ModelVaryingType modelVaryingType = ModelVaryingType::Monotone;
			double MaximumLengthU = 20;
			double Dsdu = 1;
			double EpsilonUStepSize = 0.01;
			int MaximumIterations = 50;
			double EpsilonZStepSize = 0.01;

			StochastSettingsSet* StochastSet = new StochastSettingsSet();

			bool IsValid();
		};
	}
}

