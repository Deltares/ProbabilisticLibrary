#pragma once
#include "../Model/RandomSettings.h"
#include "RandomProvider.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class RandomSettingsWrapper
			{
			private:
				RandomSettings* m_settings;

			public:
				RandomSettingsWrapper()
				{
					m_settings = new RandomSettings();
					RandomProvider::initialize();
				}
				~RandomSettingsWrapper() { this->!RandomSettingsWrapper(); }
				!RandomSettingsWrapper() { delete m_settings; }

				property int Seed
				{
					int get() { return m_settings->Seed; }
					void set(int value) { m_settings->Seed = value; }
				}

				property int SeedB
				{
					int get() { return m_settings->SeedB; }
					void set(int value) { m_settings->SeedB = value; }
				}

				property bool IsRepeatableRandom
				{
					bool get() { return m_settings->IsRepeatableRandom; }
					void set(bool value) { m_settings->IsRepeatableRandom = value; }
				}

				RandomSettings* GetSettings()
				{
					return m_settings;
				}
			};
		}
	}
}


