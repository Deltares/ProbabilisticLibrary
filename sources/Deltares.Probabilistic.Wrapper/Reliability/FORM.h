#pragma once

#include "FORMSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/FORM.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class FORM : public ReliabilityMethod
			{
			private:
				SharedPointerProvider<Reliability::FORM>* shared = new SharedPointerProvider(new Reliability::FORM());
			public:
				FORM() {}
				~FORM() { this->!FORM(); }
				!FORM() { delete shared; }

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					shared->object->Settings = Settings->GetSettings();
					return shared->object;
				}

				FORMSettings^ Settings = gcnew FORMSettings();

				System::Object^ GetSettings() override { return Settings; }

				bool IsValid() override { return Settings->IsValid(); }

				void Stop()	override { shared->object->Stop(); }

				bool IsStopped() override { return shared->object->isStopped(); }
			};
		}
	}
}



