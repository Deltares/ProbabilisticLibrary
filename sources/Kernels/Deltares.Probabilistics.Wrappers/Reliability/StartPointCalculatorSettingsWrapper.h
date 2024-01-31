#pragma once

#include "../Statistics/StochastSettingsWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/StartPointCalculatorSettings.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

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
				Reliability::StartPointCalculatorSettings* settings;
				SharedPointerProvider<Reliability::StartPointCalculatorSettings>* sharedPointer = new SharedPointerProvider<Reliability::StartPointCalculatorSettings>();

			public:
				StartPointCalculatorSettingsWrapper()
				{
					settings = new Reliability::StartPointCalculatorSettings();
				}
				~StartPointCalculatorSettingsWrapper() { this->!StartPointCalculatorSettingsWrapper(); }
				!StartPointCalculatorSettingsWrapper() { delete sharedPointer; }

				property double MaximumLengthStartPoint
				{
					double get() { return settings->MaximumLengthStartPoint; }
					void set(double value) { settings->MaximumLengthStartPoint = value; }
				}

				property double GradientStepSize
				{
					double get() { return settings->GradientStepSize; }
					void set(double value) { settings->GradientStepSize = value; }
				}

				property double RadiusSphereSearch
				{
					double get() { return settings->RadiusSphereSearch; }
					void set(double value) { settings->RadiusSphereSearch = value; }
				}

				property Kernels::StartMethodType StartMethod
				{
					Kernels::StartMethodType get()
					{
						switch (settings->StartMethod)
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
						case Kernels::StartMethodType::None: settings->StartMethod = Reliability::None; break;
						case Kernels::StartMethodType::RaySearch: settings->StartMethod = Reliability::RaySearch; break;
						case Kernels::StartMethodType::SensitivitySearch: settings->StartMethod = Reliability::SensitivitySearch; break;
						case Kernels::StartMethodType::SphereSearch: settings->StartMethod = Reliability::SphereSearch; break;
						default: throw gcnew System::NotSupportedException("Start method");
						}
					}
				}

				System::Collections::Generic::List<StochastSettingsWrapper^>^ StochastSettings = gcnew System::Collections::Generic::List<StochastSettingsWrapper^>();

				std::shared_ptr<Reliability::StartPointCalculatorSettings> GetSettings()
				{
					std::shared_ptr<Reliability::StartPointCalculatorSettings> m_settings = sharedPointer->getSharedPointer(settings);

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


