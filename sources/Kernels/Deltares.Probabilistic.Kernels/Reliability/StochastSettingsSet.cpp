#include "StochastSettingsSet.h"
#include "StochastSettings.h"

#include <memory>

namespace Deltares
{
	namespace Reliability
	{
		void StochastSettingsSet::loadStochastPoint(StochastPoint* stochastPoint)
		{
			this->StochastCount = stochastPoint->Alphas.size();
			this->StochastSettings.clear();

			for (int i = 0; i < this->StochastCount; i++)
			{
				Deltares::Reliability::StochastSettings* settings = new Deltares::Reliability::StochastSettings();
				settings->StartValue = stochastPoint->Alphas[i]->U;
				settings->setStochast(stochastPoint->Alphas[i]->Stochast);

				this->StochastSettings.push_back(settings);
			}
		}

		Sample* StochastSettingsSet::getSample()
		{
			Sample* sample = new Sample(this->VaryingStochastCount);

			for (int i = 0; i < this->VaryingStochastCount; i++)
			{
				sample->Values[i] = this->VaryingStochastSettings[i]->StartValue;
			}

			return sample;
		}
	}
}




