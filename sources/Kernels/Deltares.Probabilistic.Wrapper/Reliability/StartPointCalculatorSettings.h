#pragma once

#include "../../Deltares.Probabilistic/Reliability/StartPointCalculatorSettings.h"
#include "../../Deltares.Probabilistic/Reliability/StochastSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public enum class StartMethodType
			{
				None,
				RaySearch,
				SensitivitySearch,
				SphereSearch
			};

			public ref class StartPointCalculatorSettings
			{
			private:
				Reliability::StartPointCalculatorSettings* settings;
				SharedPointerProvider<Reliability::StartPointCalculatorSettings>* sharedPointer = new SharedPointerProvider<Reliability::StartPointCalculatorSettings>();

			public:
				StartPointCalculatorSettings()
				{
					settings = new Reliability::StartPointCalculatorSettings();
				}
				~StartPointCalculatorSettings() { this->!StartPointCalculatorSettings(); }
				!StartPointCalculatorSettings() { delete sharedPointer; }

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

				property Wrappers::StartMethodType StartMethod
				{
					Wrappers::StartMethodType get()
					{
						switch (settings->StartMethod)
						{
						case Deltares::Reliability::StartMethodType::None: return Wrappers::StartMethodType::None;
						case Deltares::Reliability::StartMethodType::RaySearch: return Wrappers::StartMethodType::RaySearch;
						case Deltares::Reliability::StartMethodType::SensitivitySearch: return Wrappers::StartMethodType::SensitivitySearch;
						case Deltares::Reliability::StartMethodType::SphereSearch: return Wrappers::StartMethodType::SphereSearch;
						default: throw gcnew System::NotSupportedException("start method");
						}
					}
					void set(Wrappers::StartMethodType value)
					{
						switch (value)
						{
						case Wrappers::StartMethodType::None: settings->StartMethod = Reliability::None; break;
						case Wrappers::StartMethodType::RaySearch: settings->StartMethod = Reliability::RaySearch; break;
						case Wrappers::StartMethodType::SensitivitySearch: settings->StartMethod = Reliability::SensitivitySearch; break;
						case Wrappers::StartMethodType::SphereSearch: settings->StartMethod = Reliability::SphereSearch; break;
						default: throw gcnew System::NotSupportedException("Start method");
						}
					}
				}

				System::Collections::Generic::List<Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Wrappers::StochastSettings^>();

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


