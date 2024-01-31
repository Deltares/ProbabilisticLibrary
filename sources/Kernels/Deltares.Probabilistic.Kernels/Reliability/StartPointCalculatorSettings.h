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
			double GradientStepSize = 4;
			double RadiusSphereSearch = 10;

			std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();
		};
	}
}
