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
			~StochastSettingsSet()
			{
				int k = 1;
			}


			std::vector<StochastSettings*> StochastSettings;
			int getStochastCount()
			{
				return static_cast<int>(this->StochastSettings.size());
			}

			std::vector<Deltares::Reliability::StochastSettings*> VaryingStochastSettings;
			int getVaryingStochastCount()
			{
				return static_cast<int>(this->VaryingStochastSettings.size());
			}

			Sample* getSample();
		};
	}
}

