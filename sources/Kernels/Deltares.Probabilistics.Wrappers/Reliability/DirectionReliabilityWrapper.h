#pragma once
#include "DirectionReliabilitySettingsWrapper.h"
#include "ReliabilityMethodWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/DirectionReliability.h"

ref class DirectionReliabilityWrapper
{
};

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class DirectionReliabilityWrapper : public ReliabilityMethodWrapper
			{
			private:
				Reliability::DirectionReliability* method;

			public:
				DirectionReliabilityWrapper()
				{
					method = new Reliability::DirectionReliability();
				}
				~DirectionReliabilityWrapper() { this->!DirectionReliabilityWrapper(); }
				!DirectionReliabilityWrapper() { delete method; }

				Reliability::ReliabilityMethod* GetReliabilityMethod() override
				{
					method->Settings = Settings->GetSettings();
					return method;
				}

				DirectionReliabilitySettingsWrapper^ Settings = gcnew DirectionReliabilitySettingsWrapper();
			};
		}
	}
}


