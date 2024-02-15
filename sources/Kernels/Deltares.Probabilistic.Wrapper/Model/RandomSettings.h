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
				SharedPointerProvider<Deltares::Models::RandomSettings>* shared = new SharedPointerProvider(new Deltares::Models::RandomSettings());

			public:
				RandomSettings()
				{
					RandomProvider::initialize();
				}
				~RandomSettings() { this->!RandomSettings(); }
				!RandomSettings() { delete shared; }

				property int Seed
				{
					int get() { return shared->object->Seed; }
					void set(int value) { shared->object->Seed = value; }
				}

				property int SeedB
				{
					int get() { return shared->object->SeedB; }
					void set(int value) { shared->object->SeedB = value; }
				}

				property bool IsRepeatableRandom
				{
					bool get() { return shared->object->IsRepeatableRandom; }
					void set(bool value) { shared->object->IsRepeatableRandom = value; }
				}

				std::shared_ptr<Models::RandomSettings> GetSettings()
				{
					return shared->object;
				}
			};
		}
	}
}


