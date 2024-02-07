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
				Reliability::CrudeMonteCarloSettings* settings;
				SharedPointerProvider<Reliability::CrudeMonteCarloSettings>* sharedPointer = new SharedPointerProvider<Reliability::CrudeMonteCarloSettings>();
			public:
				CrudeMonteCarloSettings()
				{
					settings = new Reliability::CrudeMonteCarloSettings();
					settings->randomSettings = RandomSettings->GetSettings();
				}
				~CrudeMonteCarloSettings() { this->!CrudeMonteCarloSettings(); }
				!CrudeMonteCarloSettings() { delete sharedPointer; }

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

				System::Collections::Generic::List<Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Wrappers::StochastSettings^>();

				std::shared_ptr<Reliability::CrudeMonteCarloSettings> GetSettings()
				{
					std::shared_ptr<Reliability::CrudeMonteCarloSettings> m_settings = sharedPointer->getSharedPointer(settings);

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


