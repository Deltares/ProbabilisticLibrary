#pragma once
#include "DirectionReliabilitySettingsWrapper.h"
#include "ReliabilityMethodWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/DirectionReliability.h"

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
				SharedPointerProvider<Reliability::DirectionReliability>* sharedPointer = new SharedPointerProvider<Reliability::DirectionReliability>();
			public:
				DirectionReliabilityWrapper()
				{
					method = new Reliability::DirectionReliability();
				}
				~DirectionReliabilityWrapper() { this->!DirectionReliabilityWrapper(); }
				!DirectionReliabilityWrapper() { delete sharedPointer; }

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					std::shared_ptr<Reliability::DirectionReliability> m_method = sharedPointer->getSharedPointer(method);

					m_method->Settings = Settings->GetSettings();
					return m_method;
				}

				DirectionReliabilitySettingsWrapper^ Settings = gcnew DirectionReliabilitySettingsWrapper();
			};
		}
	}
}


