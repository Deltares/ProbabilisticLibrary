#pragma once
#include "StochastSettings.h"

namespace Deltares
{
	namespace Reliability
	{
		class StochastListSettings
		{
		public:
			Deltares::Reliability::StochastSettings** StochastSettings;
			int StochastCount = 0;

			Deltares::Reliability::StochastSettings** VaryingStochastSettings;
			int VaryingStochastCount = 0;

			void useStochastSettingsFrom(StochastListSettings* other);
		};
	}
}

