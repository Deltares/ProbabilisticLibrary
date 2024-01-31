#pragma once

#include "FORMSettingsWrapper.h"
#include "ReliabilityMethodWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/FORM.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class FORMWrapper : public ReliabilityMethodWrapper
			{
			private:
				Reliability::FORM* form;
				SharedPointerProvider<Reliability::FORM>* sharedPointer = new SharedPointerProvider<Reliability::FORM>();
			public:
				FORMWrapper()
				{
					form = new Reliability::FORM();
				}
				~FORMWrapper() { this->!FORMWrapper(); }
				!FORMWrapper() { delete sharedPointer; }

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					std::shared_ptr<Reliability::FORM> m_form = sharedPointer->getSharedPointer(form);

					m_form->Settings = Settings->GetSettings();
					return m_form;
				}

				FORMSettingsWrapper^ Settings = gcnew FORMSettingsWrapper();
			};
		}
	}
}



