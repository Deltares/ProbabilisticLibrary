#pragma once
#include "DirectionReliabilitySettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/DirectionReliability.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class DirectionReliability : public ReliabilityMethod
			{
			private:
				Reliability::DirectionReliability* method;
				SharedPointerProvider<Reliability::DirectionReliability>* sharedPointer = new SharedPointerProvider<Reliability::DirectionReliability>();
			public:
				DirectionReliability()
				{
					method = new Reliability::DirectionReliability();
				}
				~DirectionReliability() { this->!DirectionReliability(); }
				!DirectionReliability() { delete sharedPointer; }

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					std::shared_ptr<Reliability::DirectionReliability> m_method = sharedPointer->getSharedPointer(method);

					m_method->Settings = Settings->GetSettings();
					return m_method;
				}

				DirectionReliabilitySettings^ Settings = gcnew DirectionReliabilitySettings();
			};
		}
	}
}


