#include "StochastSettingsSet.h"
#include "StochastSettings.h"

#include <memory>

namespace Deltares
{
	namespace Reliability
	{
		void StochastSettingsSet::loadStochastPoint(std::shared_ptr<StochastPoint> stochastPoint)
		{
			this->StochastSettings.clear();

			for (size_t i = 0; i < stochastPoint->Alphas.size(); i++)
			{
				std::shared_ptr<Deltares::Reliability::StochastSettings> settings = std::make_shared<Deltares::Reliability::StochastSettings>();
				settings->StartValue = stochastPoint->Alphas[i]->U;
				settings->stochast = stochastPoint->Alphas[i]->Stochast;

				this->StochastSettings.push_back(settings);
			}
		}

		std::shared_ptr<Sample> StochastSettingsSet::getSample()
		{
			std::shared_ptr<Sample> sample = std::make_shared<Sample>(this->getVaryingStochastCount());

			for (int i = 0; i < this->getVaryingStochastCount(); i++)
			{
				sample->Values[i] = this->VaryingStochastSettings[i]->StartValue;
			}

			return sample;
		}
	}
}




