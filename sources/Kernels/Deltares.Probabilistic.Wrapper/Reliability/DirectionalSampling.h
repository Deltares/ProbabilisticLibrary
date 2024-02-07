#pragma once

#include "DirectionalSamplingSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/DirectionalSampling.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class DirectionalSampling : public ReliabilityMethod
			{
			private:
				Reliability::DirectionalSampling* directionalSampling;
				SharedPointerProvider<Reliability::DirectionalSampling>* sharedPointer = new SharedPointerProvider<Reliability::DirectionalSampling>();
			public:
				DirectionalSampling()
				{
					directionalSampling = new Reliability::DirectionalSampling();
				}
				~DirectionalSampling() { this->!DirectionalSampling(); }
				!DirectionalSampling() { delete sharedPointer; }

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					std::shared_ptr<Reliability::DirectionalSampling> m_directionalSampling = sharedPointer->getSharedPointer(directionalSampling);

					m_directionalSampling->Settings = Settings->GetSettings();
					return m_directionalSampling;
				}

				DirectionalSamplingSettings^ Settings = gcnew DirectionalSamplingSettings();
			};
		}
	}
}





