#pragma once

#include "../Statistics/StochastSettingsWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/StartPointCalculatorSettings.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/StochastSettings.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public enum class StartMethodType
			{
				None,
				RaySearch,
				SensitivitySearch,
				SphereSearch
			};

			public ref class StartPointCalculatorSettingsWrapper
			{
			private:
				Reliability::StartPointCalculatorSettings* m_settings;

			public:
				StartPointCalculatorSettingsWrapper()
				{
					m_settings = new Reliability::StartPointCalculatorSettings();
				}
				~StartPointCalculatorSettingsWrapper() { this->!StartPointCalculatorSettingsWrapper(); }
				!StartPointCalculatorSettingsWrapper() { delete m_settings; }

				property double MaximumLengthStartPoint
				{
					double get() { return m_settings->MaximumLengthStartPoint; }
					void set(double value) { m_settings->MaximumLengthStartPoint = value; }
				}

				property double GradientStepSize
				{
					double get() { return m_settings->GradientStepSize; }
					void set(double value) { m_settings->GradientStepSize = value; }
				}

				property double RadiusSphereSearch
				{
					double get() { return m_settings->RadiusSphereSearch; }
					void set(double value) { m_settings->RadiusSphereSearch = value; }
				}

				property Kernels::StartMethodType StartMethod
				{
					Kernels::StartMethodType get()
					{
						switch (m_settings->StartMethod)
						{
						case Deltares::Reliability::StartMethodType::None: return Kernels::StartMethodType::None;
						case Deltares::Reliability::StartMethodType::RaySearch: return Kernels::StartMethodType::RaySearch;
						case Deltares::Reliability::StartMethodType::SensitivitySearch: return Kernels::StartMethodType::SensitivitySearch;
						case Deltares::Reliability::StartMethodType::SphereSearch: return Kernels::StartMethodType::SphereSearch;
						default: throw gcnew System::NotSupportedException("start method");
						}
					}
					void set(Kernels::StartMethodType value)
					{
						switch (value)
						{
						case Kernels::StartMethodType::None: m_settings->StartMethod = Reliability::None; break;
						case Kernels::StartMethodType::RaySearch: m_settings->StartMethod = Reliability::RaySearch; break;
						case Kernels::StartMethodType::SensitivitySearch: m_settings->StartMethod = Reliability::SensitivitySearch; break;
						case Kernels::StartMethodType::SphereSearch: m_settings->StartMethod = Reliability::SphereSearch; break;
						default: throw gcnew System::NotSupportedException("Start method");
						}
					}
				}


				System::Collections::Generic::List<StochastSettingsWrapper^>^ StochastSettings = gcnew System::Collections::Generic::List<StochastSettingsWrapper^>();

				Reliability::StartPointCalculatorSettings* GetSettings()
				{
					m_settings->StochastSet = new Deltares::Reliability::StochastSettingsSet();

					m_settings->StochastSet->StochastCount = StochastSettings->Count;
					m_settings->StochastSet->StochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						m_settings->StochastSet->StochastSettings.push_back(StochastSettings[i]->getSettings());
					}

					return m_settings;
				}
			};
		}
	}
}


