#pragma once
#include "StochastSettings.h"
#include "../Model/Sample.h"
#include "../Model/StochastPoint.h"

#include <memory>

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

			std::vector<StochastSettings*> StochastSettings;
			int StochastCount = 0;

			std::vector<Deltares::Reliability::StochastSettings*> VaryingStochastSettings;
			int VaryingStochastCount = 0;

			Sample* getSample();
		};
	}
}

