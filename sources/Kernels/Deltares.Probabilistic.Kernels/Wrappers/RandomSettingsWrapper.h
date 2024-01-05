#pragma once
#include "../pch.h"
#include "../Model/RandomSettings.h"

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
				RandomSettingsWrapper() { m_settings = new RandomSettings(); }
				~RandomSettingsWrapper() { this->!RandomSettingsWrapper(); }
				!RandomSettingsWrapper() { delete m_settings; }

				property int Seed
				{
					int get() { return m_settings->Seed; }
					void set(int value) { m_settings->Seed = value; }
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


