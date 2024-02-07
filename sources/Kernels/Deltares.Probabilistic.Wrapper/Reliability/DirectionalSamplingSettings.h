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
				Reliability::DirectionalSamplingSettings* settings;
				SharedPointerProvider<Reliability::DirectionalSamplingSettings>* sharedPointer = new SharedPointerProvider<Reliability::DirectionalSamplingSettings>();
			public:
				DirectionalSamplingSettings()
				{
					settings = new Reliability::DirectionalSamplingSettings();
				}
				~DirectionalSamplingSettings() { this->!DirectionalSamplingSettings(); }
				!DirectionalSamplingSettings() { delete sharedPointer; }

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

				Wrappers::RandomSettings^ RandomSettings = gcnew Wrappers::RandomSettings();

				DirectionReliabilitySettings^ DirectionalSettings = gcnew DirectionReliabilitySettings();

				System::Collections::Generic::List<Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Wrappers::StochastSettings^>();

				std::shared_ptr<Reliability::DirectionalSamplingSettings> GetSettings()
				{
					std::shared_ptr<Reliability::DirectionalSamplingSettings> m_settings = sharedPointer->getSharedPointer(settings);

					m_settings->RandomSettings = RandomSettings->GetSettings();
					m_settings->DirectionSettings = DirectionalSettings->GetSettings();

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


