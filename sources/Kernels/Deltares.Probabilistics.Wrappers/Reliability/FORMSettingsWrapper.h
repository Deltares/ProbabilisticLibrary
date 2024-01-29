#pragma once
#include "GradientCalculatorSettingsWrapper.h"
#include "../Statistics/StochastSettingsWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/FORMSettings.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/StochastSettings.h"
#include "StartPointCalculatorSettingsWrapper.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class FORMSettingsWrapper
			{
			private:
				Reliability::FORMSettings* m_settings;

			public:
				FORMSettingsWrapper()
				{
					m_settings = new Reliability::FORMSettings();
				}
				~FORMSettingsWrapper() { this->!FORMSettingsWrapper(); }
				!FORMSettingsWrapper() { delete m_settings; }

				property int MaximumIterations
				{
					int get() { return m_settings->MaximumIterations; }
					void set(int value) { m_settings->MaximumIterations = value; }
				}

				property int RelaxationLoops
				{
					int get() { return m_settings->RelaxationLoops; }
					void set(int value) { m_settings->RelaxationLoops = value; }
				}

				property double RelaxationFactor
				{
					double get() { return m_settings->RelaxationFactor; }
					void set(double value) { m_settings->RelaxationFactor = value; }
				}

				property double EpsilonBeta
				{
					double get() { return m_settings->EpsilonBeta; }
					void set(double value) { m_settings->EpsilonBeta = value; }
				}

				StartPointCalculatorSettingsWrapper^ StartPointCalculatorSettings = gcnew StartPointCalculatorSettingsWrapper();

				GradientCalculatorSettingsWrapper^ GradientCalculatorSettings = gcnew GradientCalculatorSettingsWrapper();

				System::Collections::Generic::List<StochastSettingsWrapper^>^ StochastSettings = gcnew System::Collections::Generic::List<StochastSettingsWrapper^>();

				Reliability::FORMSettings* GetSettings()
				{
					m_settings->StochastSet = new Deltares::Reliability::StochastSettingsSet();

					m_settings->StochastSet->StochastCount = StochastSettings->Count;
					m_settings->StochastSet->StochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						m_settings->StochastSet->StochastSettings.push_back(StochastSettings[i]->getSettings());
					}

					m_settings->StartPointSettings = StartPointCalculatorSettings->GetSettings();
					m_settings->StartPointSettings->StochastSet = m_settings->StochastSet;

					m_settings->GradientSettings = GradientCalculatorSettings->GetSettings();

					return m_settings;
				}
			};
		}
	}
}


