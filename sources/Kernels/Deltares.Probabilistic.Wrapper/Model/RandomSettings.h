#pragma once
#include "../../Deltares.Probabilistic/Model/RandomSettings.h"
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

			public ref class RandomSettings
			{
			private:
				Models::RandomSettings* m_settings;
				SharedPointerProvider<Deltares::Models::RandomSettings>* sharedPointer = new SharedPointerProvider<Deltares::Models::RandomSettings>();

			public:
				RandomSettings()
				{
					m_settings = new Deltares::Models::RandomSettings();
					RandomProvider::initialize();
				}
				~RandomSettings() { this->!RandomSettings(); }
				!RandomSettings() { delete sharedPointer; }

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

				std::shared_ptr<Models::RandomSettings> GetSettings()
				{
					return sharedPointer->getSharedPointer(m_settings);
				}
			};
		}
	}
}


