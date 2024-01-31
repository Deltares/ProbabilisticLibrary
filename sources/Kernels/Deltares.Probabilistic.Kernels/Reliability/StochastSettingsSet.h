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
			void loadStochastPoint(std::shared_ptr<StochastPoint> stochastPoint);
		public:
			StochastSettingsSet() {}
			StochastSettingsSet(std::shared_ptr<StochastPoint> stochastPoint)
			{
				loadStochastPoint(stochastPoint);
			}

			std::vector<std::shared_ptr<StochastSettings>> StochastSettings;
			int getStochastCount()
			{
				return static_cast<int>(this->StochastSettings.size());
			}

			std::vector<std::shared_ptr<Deltares::Reliability::StochastSettings>> VaryingStochastSettings;
			int getVaryingStochastCount()
			{
				return static_cast<int>(this->VaryingStochastSettings.size());
			}

			std::shared_ptr<Sample> getSample();
		};
	}
}

