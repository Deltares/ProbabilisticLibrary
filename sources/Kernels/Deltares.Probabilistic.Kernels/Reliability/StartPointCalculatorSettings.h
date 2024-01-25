#pragma once
#include "StochastSettingsSet.h"

namespace Deltares
{
	namespace Reliability
	{
		enum StartMethodType { None, RaySearch, SensitivitySearch, SphereSearch };

		class StartPointCalculatorSettings
		{
		public:
			StartMethodType StartMethod = StartMethodType::None;
			double MaximumLengthStartPoint = 6;
			double RadiusSphereSearch = 10;

			StochastSettingsSet* StochastSet = new StochastSettingsSet();
		};
	}
}
