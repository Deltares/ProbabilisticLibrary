#pragma once
#include "../../Deltares.Probabilistic.Kernels/Reliability/FORMSettings.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/StochastSettings.h"
#include "../Model/GradientCalculatorSettingsWrapper.h"
#include "../Utils/SharedPointerProvider.h"
#include "StartPointCalculatorSettingsWrapper.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class FORMSettingsWrapper
			{
			private:
				Reliability::FORMSettings* settings;
				SharedPointerProvider<Reliability::FORMSettings>* sharedPointer = new SharedPointerProvider<Reliability::FORMSettings>();
			public:
				FORMSettingsWrapper()
				{
					settings = new Reliability::FORMSettings();
				}
				~FORMSettingsWrapper() { this->!FORMSettingsWrapper(); }
				!FORMSettingsWrapper() { delete sharedPointer; }

				property int MaximumIterations
				{
					int get() { return settings->MaximumIterations; }
					void set(int value) { settings->MaximumIterations = value; }
				}

				property int RelaxationLoops
				{
					int get() { return settings->RelaxationLoops; }
					void set(int value) { settings->RelaxationLoops = value; }
				}

				property double RelaxationFactor
				{
					double get() { return settings->RelaxationFactor; }
					void set(double value) { settings->RelaxationFactor = value; }
				}

				property double EpsilonBeta
				{
					double get() { return settings->EpsilonBeta; }
					void set(double value) { settings->EpsilonBeta = value; }
				}

				StartPointCalculatorSettingsWrapper^ StartPointCalculatorSettings = gcnew StartPointCalculatorSettingsWrapper();

				GradientCalculatorSettingsWrapper^ GradientCalculatorSettings = gcnew GradientCalculatorSettingsWrapper();

				System::Collections::Generic::List<StochastSettingsWrapper^>^ StochastSettings = gcnew System::Collections::Generic::List<StochastSettingsWrapper^>();

				std::shared_ptr<Reliability::FORMSettings> GetSettings()
				{
					std::shared_ptr<Reliability::FORMSettings> m_settings = sharedPointer->getSharedPointer(settings);

					m_settings->StochastSet->StochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						m_settings->StochastSet->StochastSettings.push_back(StochastSettings[i]->GetSettings());
					}

					m_settings->StartPointSettings = StartPointCalculatorSettings->GetSettings();
					m_settings->StartPointSettings->StochastSet = settings->StochastSet;

					m_settings->GradientSettings = GradientCalculatorSettings->GetSettings();

					return m_settings;
				}
			};
		}
	}
}


