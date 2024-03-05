#pragma once
#include <vector>
#include "StochastSettingsSet.h"

namespace Deltares
{
	namespace Reliability
	{
		enum StartMethodType { None, One, RaySearch, SensitivitySearch, SphereSearch, GivenVector };

		class StartPointCalculatorSettings
		{
		public:
			StartMethodType StartMethod = StartMethodType::None;
			double MaximumLengthStartPoint = 6;
			double GradientStepSize = 4;
			double RadiusSphereSearch = 10;
			double dsdu = 1;

			std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();
			std::vector<double> startVector;
		};
	}
}
