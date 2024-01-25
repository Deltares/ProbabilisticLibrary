#pragma once

#include "../../Deltares.Probabilistic.Kernels/Model/GradientSettings.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
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

			public:
				GradientCalculatorSettingsWrapper()
				{
					m_settings = new Models::GradientSettings();
				}
				~GradientCalculatorSettingsWrapper() { this->!GradientCalculatorSettingsWrapper(); }
				!GradientCalculatorSettingsWrapper() { delete m_settings; }

				property double StepSize
				{
					double get() { return m_settings->StepSize; }
					void set(double value) { m_settings->StepSize = value; }
				}

				property Kernels::GradientType GradientType
				{
					Kernels::GradientType get()
					{
						switch (m_settings->GradientType)
						{
						case Deltares::Models::GradientType::OneDirection: return Kernels::GradientType::OneDirection;
						case Deltares::Models::GradientType::TwoDirections: return Kernels::GradientType::TwoDirections;
						default: throw gcnew System::NotSupportedException("gradient type");
						}
					}
					void set(Kernels::GradientType value)
					{
						switch (value)
						{
						case Kernels::GradientType::OneDirection: m_settings->GradientType = Models::GradientType::OneDirection; break;
						case Kernels::GradientType::TwoDirections: m_settings->GradientType = Models::GradientType::TwoDirections; break;
						default: throw gcnew System::NotSupportedException("gradient type");
						}
					}
				}

				Models::GradientSettings* GetSettings()
				{
					return m_settings;
				}
			};
		}
	}
}



