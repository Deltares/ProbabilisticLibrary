#pragma once

#include "../Model/RandomSettingsWrapper.h"
#include "../Statistics/StochastSettingsWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/CrudeMonteCarloSettings.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/StochastSettings.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class CrudeMonteCarloSettingsWrapper
			{
			private:
				Reliability::CrudeMonteCarloSettings* m_settings;

			public:
				CrudeMonteCarloSettingsWrapper()
				{
					m_settings = new Reliability::CrudeMonteCarloSettings();
					m_settings->randomSettings = RandomSettings->GetSettings();
				}
				~CrudeMonteCarloSettingsWrapper() { this->!CrudeMonteCarloSettingsWrapper(); }
				!CrudeMonteCarloSettingsWrapper() { delete m_settings; }

				property int MinimumSamples
				{
					int get() { return m_settings->MinimumSamples; }
					void set(int value) { m_settings->MinimumSamples = value; }
				}

				property int MaximumSamples
				{
					int get() { return m_settings->MaximumSamples; }
					void set(int value) { m_settings->MaximumSamples = value; }
				}

				property double VariationCoefficient
				{
					double get() { return m_settings->VariationCoefficient; }
					void set(double value) { m_settings->VariationCoefficient = value; }
				}

				RandomSettingsWrapper^ RandomSettings = gcnew RandomSettingsWrapper();

				System::Collections::Generic::List<StochastSettingsWrapper^>^ StochastSettings = gcnew System::Collections::Generic::List<StochastSettingsWrapper^>();

				Reliability::CrudeMonteCarloSettings* GetSettings()
				{
					m_settings->StochastSet = new Deltares::Reliability::StochastSettingsSet();

					m_settings->StochastSet->StochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						m_settings->StochastSet->StochastSettings.push_back(StochastSettings[i]->getSettings());
					}

					return m_settings;
				}
			};
		}
	}
}


