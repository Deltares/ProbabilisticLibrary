#include "StochastSettingsSet.h"
#include "StochastSettings.h"

namespace Deltares
{
	namespace Reliability
	{
		void StochastSettingsSet::loadStochastPoint(StochastPoint* stochastPoint)
		{
			this->StochastCount = stochastPoint->Alphas.size();
			this->StochastSettings = new Deltares::Reliability::StochastSettings * [this->StochastCount];
			for (int i = 0; i < this->StochastCount; i++)
			{
				Deltares::Reliability::StochastSettings* settings = new Deltares::Reliability::StochastSettings();
				settings->StartValue = stochastPoint->Alphas[i]->U;
				settings->setStochast(stochastPoint->Alphas[i]->Stochast);

				this->StochastSettings[i] = settings;
			}
		}

		Sample* StochastSettingsSet::getSample()
		{
			double* values = new double[this->VaryingStochastCount];

			for (int i = 0; i < this->VaryingStochastCount; i++)
			{
				values[i] = this->VaryingStochastSettings[i]->StartValue;
			}


			return new Sample(values, this->VaryingStochastCount);
		}
	}
}




