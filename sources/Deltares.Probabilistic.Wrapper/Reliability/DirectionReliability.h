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
				SharedPointerProvider<Reliability::DirectionReliability>* shared = new SharedPointerProvider(new Reliability::DirectionReliability());
			public:
				DirectionReliability() {}
				~DirectionReliability() { this->!DirectionReliability(); }
				!DirectionReliability() { delete shared; }

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					shared->object->Settings = Settings->GetSettings();
					return shared->object;
				}

				DirectionReliabilitySettings^ Settings = gcnew DirectionReliabilitySettings();
			};
		}
	}
}


