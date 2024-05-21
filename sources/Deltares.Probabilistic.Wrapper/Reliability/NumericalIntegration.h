#pragma once

#include "NumericalIntegrationSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/NumericalIntegration.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class NumericalIntegration : public ReliabilityMethod
			{
			private:
				SharedPointerProvider<Reliability::NumericalIntegration>* shared = new SharedPointerProvider(new Reliability::NumericalIntegration());
			public:
				NumericalIntegration() {}
				~NumericalIntegration() { this->!NumericalIntegration(); }
				!NumericalIntegration() { delete shared; }

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					shared->object->Settings = *Settings->GetSettings();
					return shared->object;
				}

				NumericalIntegrationSettings^ Settings = gcnew NumericalIntegrationSettings();

				System::Object^ GetSettings() override { return Settings; }

				bool IsValid() override { return Settings->IsValid(); }

				void Stop()	override { shared->object->Stop(); }

				bool IsStopped() override { return shared->object->isStopped(); }
			};
		}
	}
}


