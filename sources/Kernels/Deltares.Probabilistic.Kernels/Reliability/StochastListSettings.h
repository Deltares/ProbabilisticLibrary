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
			int StochastSettingsCount = 0;

			Deltares::Reliability::StochastSettings** VaryingStochastSettings;
			int VaryingStochastSettingsCount = 0;
		};
	}
}

