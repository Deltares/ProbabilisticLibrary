#pragma once
#include "../../Deltares.Probabilistic.Kernels/Model/RandomSettings.h"
#include "../Utils/SharedPointerProvider.h"
#include "RandomProvider.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Models;
			using namespace Deltares::Utils::Wrappers;

			public ref class RandomSettingsWrapper
			{
			private:
				RandomSettings* m_settings;
				SharedPointerProvider<RandomSettings>* sharedPointer = new SharedPointerProvider<RandomSettings>();

			public:
				RandomSettingsWrapper()
				{
					m_settings = new RandomSettings();
					RandomProvider::initialize();
				}
				~RandomSettingsWrapper() { this->!RandomSettingsWrapper(); }
				!RandomSettingsWrapper() { delete sharedPointer; }

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

				std::shared_ptr<RandomSettings> GetSettings()
				{
					return sharedPointer->getSharedPointer(m_settings);
				}
			};
		}
	}
}


