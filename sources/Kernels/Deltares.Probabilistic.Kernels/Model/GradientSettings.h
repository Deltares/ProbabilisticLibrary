#pragma once

namespace Deltares
{
	namespace Models
	{
		enum GradientType
		{
			OneDirection,
			TwoDirections
		};

		class GradientSettings
		{
		public:
			GradientSettings() {}

			GradientType GradientType = OneDirection;
			double StepSize = 0.3;
		};
	}
}


