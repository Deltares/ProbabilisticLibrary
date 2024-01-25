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
				Reliability::FORM* m_form;

			public:
				FORMWrapper()
				{
					m_form = new Reliability::FORM();
				}
				~FORMWrapper() { this->!FORMWrapper(); }
				!FORMWrapper() { delete m_form; }

				Reliability::ReliabilityMethod* GetReliabilityMethod() override
				{
					m_form->Settings = Settings->GetSettings();
					return m_form;
				}

				FORMSettingsWrapper^ Settings = gcnew FORMSettingsWrapper();
			};
		}
	}
}



