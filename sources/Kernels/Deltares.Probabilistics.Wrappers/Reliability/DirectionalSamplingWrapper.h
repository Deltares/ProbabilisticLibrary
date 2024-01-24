#pragma once

#include "DirectionalSamplingSettingsWrapper.h"
#include "ReliabilityMethodWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/DirectionalSampling.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class DirectionalSamplingWrapper : public ReliabilityMethodWrapper
			{
			private:
				Reliability::DirectionalSampling* directionalSampling;

			public:
				DirectionalSamplingWrapper()
				{
					directionalSampling = new Reliability::DirectionalSampling();
				}
				~DirectionalSamplingWrapper() { this->!DirectionalSamplingWrapper(); }
				!DirectionalSamplingWrapper() { delete directionalSampling; }

				Reliability::ReliabilityMethod* GetReliabilityMethod() override
				{
					directionalSampling->Settings = Settings->GetSettings();
					return directionalSampling;
				}

				DirectionalSamplingSettingsWrapper^ Settings = gcnew DirectionalSamplingSettingsWrapper();
			};
		}
	}
}





