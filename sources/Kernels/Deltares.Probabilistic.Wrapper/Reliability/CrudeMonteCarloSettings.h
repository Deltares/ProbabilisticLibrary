#pragma once

#include "../../Deltares.Probabilistic/Reliability/CrudeMonteCarloSettings.h"
#include "../Model/RandomSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class CrudeMonteCarloSettings
			{
			private:
				SharedPointerProvider<Reliability::CrudeMonteCarloSettings>* shared = new SharedPointerProvider(new Reliability::CrudeMonteCarloSettings());
			public:
				CrudeMonteCarloSettings()
				{
					shared->object->randomSettings = RandomSettings->GetSettings();
				}
				~CrudeMonteCarloSettings() { this->!CrudeMonteCarloSettings(); }
				!CrudeMonteCarloSettings() { delete shared; }

				property int MinimumSamples
				{
					int get() { return shared->object->MinimumSamples; }
					void set(int value) { shared->object->MinimumSamples = value; }
				}

				property int MaximumSamples
				{
					int get() { return shared->object->MaximumSamples; }
					void set(int value) { shared->object->MaximumSamples = value; }
				}

				property double VariationCoefficient
				{
					double get() { return shared->object->VariationCoefficient; }
					void set(double value) { shared->object->VariationCoefficient = value; }
				}

				Wrappers::RandomSettings^ RandomSettings = gcnew Wrappers::RandomSettings();

				System::Collections::Generic::List<Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Wrappers::StochastSettings^>();

				std::shared_ptr<Reliability::CrudeMonteCarloSettings> GetSettings()
				{
					shared->object->StochastSet->StochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						shared->object->StochastSet->StochastSettings.push_back(StochastSettings[i]->GetSettings());
					}

					return shared->object;
				}
			};
		}
	}
}


