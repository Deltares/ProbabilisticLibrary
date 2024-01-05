#pragma once

#include "RandomSettingsWrapper.h"
#include "RunSettingsWrapper.h"
#include "../Reliability/CrudeMonteCarloSettings.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class CrudeMonteCarloSettingsWrapper
			{
			private:
				CrudeMonteCarloSettings* m_settings;

			public:
				CrudeMonteCarloSettingsWrapper()
				{
					m_settings = new CrudeMonteCarloSettings();
					m_settings->RunSettings = RunSettings->GetSettings();
					m_settings->RandomSettings = RandomSettings->GetSettings();
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

				RunSettingsWrapper^ RunSettings = gcnew RunSettingsWrapper;

				RandomSettingsWrapper^ RandomSettings = gcnew RandomSettingsWrapper;

				CrudeMonteCarloSettings* GetSettings()
				{
					return m_settings;
				}
			};
		}
	}
}


