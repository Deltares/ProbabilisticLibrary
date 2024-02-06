#pragma once

#include <memory>

#include "../../Deltares.Probabilistic.Kernels/Model/GradientSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			public enum class GradientType
			{
				OneDirection,
				TwoDirections
			};

			public ref class GradientCalculatorSettingsWrapper
			{
			private:
				Models::GradientSettings* m_settings;
				Utils::Wrappers::SharedPointerProvider<Models::GradientSettings>* sharedPointer;
			public:
				GradientCalculatorSettingsWrapper()
				{
					m_settings = new Models::GradientSettings();
					sharedPointer = new Utils::Wrappers::SharedPointerProvider<Models::GradientSettings>();
				}
				~GradientCalculatorSettingsWrapper() { this->!GradientCalculatorSettingsWrapper(); }
				!GradientCalculatorSettingsWrapper() { delete sharedPointer; }

				property double StepSize
				{
					double get() { return m_settings->StepSize; }
					void set(double value) { m_settings->StepSize = value; }
				}

				property Wrappers::GradientType GradientType
				{
					Wrappers::GradientType get()
					{
						switch (m_settings->GradientType)
						{
						case Deltares::Models::GradientType::OneDirection: return Wrappers::GradientType::OneDirection;
						case Deltares::Models::GradientType::TwoDirections: return Wrappers::GradientType::TwoDirections;
						default: throw gcnew System::NotSupportedException("gradient type");
						}
					}
					void set(Wrappers::GradientType value)
					{
						switch (value)
						{
						case Wrappers::GradientType::OneDirection: m_settings->GradientType = Models::GradientType::OneDirection; break;
						case Wrappers::GradientType::TwoDirections: m_settings->GradientType = Models::GradientType::TwoDirections; break;
						default: throw gcnew System::NotSupportedException("gradient type");
						}
					}
				}

				std::shared_ptr<Models::GradientSettings> GetSettings()
				{
					return sharedPointer->getSharedPointer(m_settings);
				}
			};
		}
	}
}



