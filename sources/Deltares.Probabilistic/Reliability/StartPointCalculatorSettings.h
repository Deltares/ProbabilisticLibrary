#pragma once
#include "StochastSettingsSet.h"

namespace Deltares
{
	namespace Reliability
	{
		enum StartMethodType { None, One, RaySearch, SensitivitySearch, SphereSearch };

		class StartPointCalculatorSettings
		{
		public:
			StartMethodType StartMethod = StartMethodType::None;
			double MaximumLengthStartPoint = 6;
			double GradientStepSize = 4;
			double RadiusSphereSearch = 10;
			double dsdu = 1;

			bool isValid()
			{
				return StartMethod == StartMethodType::None ||
					StartMethod == StartMethodType::RaySearch && MaximumLengthStartPoint >= 1 ||
					StartMethod == StartMethodType::SensitivitySearch && MaximumLengthStartPoint >= 1 ||
					StartMethod == StartMethodType::SphereSearch && RadiusSphereSearch >= 0.1;
			}

			std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();
		};
	}
}
