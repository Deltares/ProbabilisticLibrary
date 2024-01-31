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
				SharedPointerProvider<Reliability::DirectionalSampling>* sharedPointer = new SharedPointerProvider<Reliability::DirectionalSampling>();
			public:
				DirectionalSamplingWrapper()
				{
					directionalSampling = new Reliability::DirectionalSampling();
				}
				~DirectionalSamplingWrapper() { this->!DirectionalSamplingWrapper(); }
				!DirectionalSamplingWrapper() { delete sharedPointer; }

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					std::shared_ptr<Reliability::DirectionalSampling> m_directionalSampling = sharedPointer->getSharedPointer(directionalSampling);

					m_directionalSampling->Settings = Settings->GetSettings();
					return m_directionalSampling;
				}

				DirectionalSamplingSettingsWrapper^ Settings = gcnew DirectionalSamplingSettingsWrapper();
			};
		}
	}
}





