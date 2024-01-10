#pragma once
#include "CrudeMonteCarloSettingsWrapper.h"
#include "ReliabilityMethodWrapper.h"
#include "../Reliability/CrudeMonteCarlo.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class CrudeMonteCarloWrapper : public ReliabilityMethodWrapper
			{
			private:
				CrudeMonteCarlo* m_crude_monte_carlo;

			public:
				CrudeMonteCarloWrapper()
				{
					m_crude_monte_carlo = new CrudeMonteCarlo();
					m_crude_monte_carlo->Settings = Settings->GetSettings();
				}
				~CrudeMonteCarloWrapper() { this->!CrudeMonteCarloWrapper(); }
				!CrudeMonteCarloWrapper() { delete m_crude_monte_carlo; }

				ReliabilityMethod* GetReliabilityMethod() override
				{
					return m_crude_monte_carlo;
				}

				CrudeMonteCarloSettingsWrapper^ Settings = gcnew CrudeMonteCarloSettingsWrapper();
			};
		}
	}
}



