#pragma once

#include "ImportanceSamplingSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/ImportanceSampling.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			public ref class ImportanceSampling : public ReliabilityMethod
			{
			private:
				SharedPointerProvider<Reliability::ImportanceSampling>* shared = new SharedPointerProvider(new Reliability::ImportanceSampling());
			public:
				ImportanceSampling() { }
				~ImportanceSampling() { this->!ImportanceSampling(); }
				!ImportanceSampling() { delete shared; }

				ImportanceSamplingSettings^ Settings = gcnew ImportanceSamplingSettings();

				System::Object^ GetSettings() override { return Settings; }

				bool IsValid() override { return Settings->IsValid(); }

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					shared->object->Settings = Settings->GetSettings();
					return shared->object;
				}
			};
		}
	}
}



