#pragma once
#include "DirectionReliabilitySettings.h"

#include "../../Deltares.Probabilistic/Reliability/DirectionalSamplingSettings.h"
#include "../Model/RandomSettings.h"
#include "../Reliability/StochastSettings.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class DirectionalSamplingSettings
			{
			private:
				SharedPointerProvider<Reliability::DirectionalSamplingSettings>* shared = new SharedPointerProvider<Reliability::DirectionalSamplingSettings>(new Reliability::DirectionalSamplingSettings());
			public:
				DirectionalSamplingSettings() {}
				~DirectionalSamplingSettings() { this->!DirectionalSamplingSettings(); }
				!DirectionalSamplingSettings() { delete shared; }

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

				DirectionReliabilitySettings^ DirectionalSettings = gcnew DirectionReliabilitySettings();

				System::Collections::Generic::List<Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Wrappers::StochastSettings^>();

				std::shared_ptr<Reliability::DirectionalSamplingSettings> GetSettings()
				{
					shared->object->RandomSettings = RandomSettings->GetSettings();
					shared->object->DirectionSettings = DirectionalSettings->GetSettings();

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


