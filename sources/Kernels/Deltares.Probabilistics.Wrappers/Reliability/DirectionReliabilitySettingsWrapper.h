#pragma once

#include "DesignPointWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/DirectionReliabilitySettings.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public enum class VaryingType
			{
				Monotone,
				Varying
			};


			public ref class DirectionReliabilitySettingsWrapper
			{
			private:
				Reliability::DirectionReliabilitySettings* m_settings;

			public:
				DirectionReliabilitySettingsWrapper()
				{
					m_settings = new Reliability::DirectionReliabilitySettings();
				}
				~DirectionReliabilitySettingsWrapper() { this->!DirectionReliabilitySettingsWrapper(); }
				!DirectionReliabilitySettingsWrapper() { delete m_settings; }

				property double Dsdu
				{
					double get() { return m_settings->Dsdu; }
					void set(double value) { m_settings->Dsdu = value; }
				}

				property double EpsilonUStepSize
				{
					double get() { return m_settings->EpsilonUStepSize; }
					void set(double value) { m_settings->EpsilonUStepSize = value; }
				}

				property double EpsilonZStepSize
				{
					double get() { return m_settings->EpsilonZStepSize; }
					void set(double value) { m_settings->EpsilonZStepSize = value; }
				}

				property int MaximumIterations
				{
					int get() { return m_settings->MaximumIterations; }
					void set(int value) { m_settings->MaximumIterations = value; }
				}

				property Kernels::VaryingType ModelVaryingType
				{
					Kernels::VaryingType get()
					{
						switch (m_settings->modelVaryingType)
						{
						case Reliability::Monotone: return Kernels::VaryingType::Monotone;
						case Reliability::Varying: return Kernels::VaryingType::Varying;
						default: throw gcnew System::NotSupportedException("Model varying type");
						}
					}
					void set(Kernels::VaryingType value)
					{
						switch (value)
						{
						case Kernels::VaryingType::Monotone: m_settings->modelVaryingType = Reliability::Monotone; break;
						case Kernels::VaryingType::Varying: m_settings->modelVaryingType = Reliability::Varying; break;
						default: throw gcnew System::NotSupportedException("Model varying type");
						}
					}
				}

				void SetStartPoint(DesignPointWrapper^ designPoint)
				{
					m_settings->StochastSet = new Reliability::StochastSettingsSet(designPoint->getDesignPoint());
				}

				Reliability::DirectionReliabilitySettings* GetSettings()
				{
					return m_settings;
				}
			};
		}
	}
}




