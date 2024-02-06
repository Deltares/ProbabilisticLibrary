#pragma once

#include "../../Deltares.Probabilistic/Reliability/CrudeMonteCarloSettings.h"
#include "../Model/RandomSettingsWrapper.h"
#include "../Reliability/StochastSettingsWrapper.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class CrudeMonteCarloSettingsWrapper
			{
			private:
				Reliability::CrudeMonteCarloSettings* settings;
				SharedPointerProvider<Reliability::CrudeMonteCarloSettings>* sharedPointer = new SharedPointerProvider<Reliability::CrudeMonteCarloSettings>();
			public:
				CrudeMonteCarloSettingsWrapper()
				{
					settings = new Reliability::CrudeMonteCarloSettings();
					settings->randomSettings = RandomSettings->GetSettings();
				}
				~CrudeMonteCarloSettingsWrapper() { this->!CrudeMonteCarloSettingsWrapper(); }
				!CrudeMonteCarloSettingsWrapper() { delete sharedPointer; }

				property int MinimumSamples
				{
					int get() { return settings->MinimumSamples; }
					void set(int value) { settings->MinimumSamples = value; }
				}

				property int MaximumSamples
				{
					int get() { return settings->MaximumSamples; }
					void set(int value) { settings->MaximumSamples = value; }
				}

				property double VariationCoefficient
				{
					double get() { return settings->VariationCoefficient; }
					void set(double value) { settings->VariationCoefficient = value; }
				}

				RandomSettingsWrapper^ RandomSettings = gcnew RandomSettingsWrapper();

				System::Collections::Generic::List<StochastSettingsWrapper^>^ StochastSettings = gcnew System::Collections::Generic::List<StochastSettingsWrapper^>();

				std::shared_ptr<Reliability::CrudeMonteCarloSettings> GetSettings()
				{
					std::shared_ptr<Reliability::CrudeMonteCarloSettings> m_settings = sharedPointer->getSharedPointer(settings);

					m_settings->StochastSet->StochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						m_settings->StochastSet->StochastSettings.push_back(StochastSettings[i]->GetSettings());
					}

					return m_settings;
				}
			};
		}
	}
}


