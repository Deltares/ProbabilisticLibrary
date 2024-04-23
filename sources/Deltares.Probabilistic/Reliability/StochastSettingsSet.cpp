#include "StochastSettingsSet.h"
#include "StochastSettings.h"

#include <memory>

namespace Deltares
{
	namespace Reliability
	{
		void StochastSettingsSet::loadStochastPoint(std::shared_ptr<StochastPoint> stochastPoint)
		{
			this->stochastSettings.clear();

			for (size_t i = 0; i < stochastPoint->Alphas.size(); i++)
			{
				std::shared_ptr<Deltares::Reliability::StochastSettings> settings = std::make_shared<Deltares::Reliability::StochastSettings>();
				settings->StartValue = stochastPoint->Alphas[i]->U;
				settings->stochast = stochastPoint->Alphas[i]->Stochast;

				this->stochastSettings.push_back(settings);
			}
		}

		std::shared_ptr<Sample> StochastSettingsSet::getSample()
		{
			std::shared_ptr<Sample> sample = std::make_shared<Sample>(this->getVaryingStochastCount());

			for (int i = 0; i < this->getVaryingStochastCount(); i++)
			{
				sample->Values[i] = this->VaryingStochastSettings[i]->UncorrelatedStartValue;
			}

			return sample;
		}

		void StochastSettingsSet::setStartPoint(const std::shared_ptr<Sample> startPoint)
		{
			for (int i = 0; i < this->getVaryingStochastCount(); i++)
			{
				this->VaryingStochastSettings[i]->StartValue = startPoint->Values[i];
				this->VaryingStochastSettings[i]->UncorrelatedStartValue = startPoint->Values[i];

				this->AreStartValuesCorrelated = false;
			}
		}
	}
}




