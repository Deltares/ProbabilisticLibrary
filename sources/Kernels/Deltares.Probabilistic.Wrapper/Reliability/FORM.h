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
				Reliability::FORM* form;
				SharedPointerProvider<Reliability::FORM>* sharedPointer = new SharedPointerProvider<Reliability::FORM>();
			public:
				FORM()
				{
					form = new Reliability::FORM();
				}
				~FORM() { this->!FORM(); }
				!FORM() { delete sharedPointer; }

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					std::shared_ptr<Reliability::FORM> m_form = sharedPointer->getSharedPointer(form);

					m_form->Settings = Settings->GetSettings();
					return m_form;
				}

				FORMSettings^ Settings = gcnew FORMSettings();
			};
		}
	}
}



