#pragma once

#include "DesignPointWrapper.h"
#include "../../Deltares.Probabilistic/Reliability/DirectionReliabilitySettings.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public enum class VaryingType
			{
				Monotone,
				Varying
			};


			public ref class DirectionReliabilitySettingsWrapper
			{
			private:
				Reliability::DirectionReliabilitySettings* m_settings;
				SharedPointerProvider<Reliability::DirectionReliabilitySettings>* sharedPointer = new SharedPointerProvider<Reliability::DirectionReliabilitySettings>();
			public:
				DirectionReliabilitySettingsWrapper()
				{
					m_settings = new Reliability::DirectionReliabilitySettings();
				}
				~DirectionReliabilitySettingsWrapper() { this->!DirectionReliabilitySettingsWrapper(); }
				!DirectionReliabilitySettingsWrapper() { delete sharedPointer; }

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

				property Wrappers::VaryingType ModelVaryingType
				{
					Wrappers::VaryingType get()
					{
						switch (m_settings->modelVaryingType)
						{
						case Reliability::Monotone: return Wrappers::VaryingType::Monotone;
						case Reliability::Varying: return Wrappers::VaryingType::Varying;
						default: throw gcnew System::NotSupportedException("Model varying type");
						}
					}
					void set(Wrappers::VaryingType value)
					{
						switch (value)
						{
						case Wrappers::VaryingType::Monotone: m_settings->modelVaryingType = Reliability::Monotone; break;
						case Wrappers::VaryingType::Varying: m_settings->modelVaryingType = Reliability::Varying; break;
						default: throw gcnew System::NotSupportedException("Model varying type");
						}
					}
				}

				void SetStartPoint(DesignPointWrapper^ designPoint)
				{
					m_settings->StochastSet = std::make_shared<Reliability::StochastSettingsSet>(designPoint->getDesignPoint());
				}

				std::shared_ptr<Reliability::DirectionReliabilitySettings> GetSettings()
				{
					return sharedPointer->getSharedPointer(m_settings);
				}
			};
		}
	}
}




