#pragma once
#include "StochastSettings.h"
#include "../Model/Sample.h"
#include "../Model/StochastPoint.h"

namespace Deltares
{
	namespace Reliability
	{
		class StochastSettingsSet
		{
		private:
			void loadStochastPoint(StochastPoint* stochastPoint);
		public:
			StochastSettingsSet() {}
			StochastSettingsSet(StochastPoint* stochastPoint)
			{
				loadStochastPoint(stochastPoint);
			}

			Deltares::Reliability::StochastSettings** StochastSettings;
			int StochastCount = 0;

			Deltares::Reliability::StochastSettings** VaryingStochastSettings;
			int VaryingStochastCount = 0;

			Sample* getSample();
		};
	}
}

