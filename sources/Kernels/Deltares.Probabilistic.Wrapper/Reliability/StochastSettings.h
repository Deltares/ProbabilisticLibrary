#pragma once
#include <memory>

#include "../../Deltares.Probabilistic/Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public ref class StochastSettings
			{
			private:
				Deltares::Reliability::StochastSettings* settings;
				SharedPointerProvider<Deltares::Reliability::StochastSettings>* sharedPointer = new SharedPointerProvider<Deltares::Reliability::StochastSettings>();

			public:
				StochastSettings() { settings = new Deltares::Reliability::StochastSettings(); }
				~StochastSettings() { this->!StochastSettings(); }
				!StochastSettings() { delete sharedPointer; }

				property double UMin
				{
					double get() { return settings->MinValue; }
					void set(double value) { settings->MinValue = value; }
				}

				property double UMax
				{
					double get() { return settings->MaxValue; }
					void set(double value) { settings->MaxValue = value; }
				}

				property double StartValue
				{
					double get() { return settings->StartValue; }
					void set(double value) { settings->StartValue = value; }
				}

				property double VarianceFactor
				{
					double get() { return settings->VarianceFactor; }
					void set(double value) { settings->VarianceFactor = value; }
				}

				property bool IsVarianceAllowed
				{
					bool get() { return settings->IsVarianceAllowed; }
					void set(bool value) { settings->IsVarianceAllowed = value; }
				}

				property int Intervals
				{
					int get() { return settings->Intervals; }
					void set(int value) { settings->Intervals = value; }
				}

				property bool IsInitializationAllowed
				{
					bool get() { return settings->IsInitializationAllowed; }
					void set(bool value) { settings->IsInitializationAllowed = value; }
				}

				std::shared_ptr<Reliability::StochastSettings> GetSettings()
				{
					return sharedPointer->getSharedPointer(settings);
				}
			};
		}
	}
}


