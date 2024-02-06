#pragma once

#include "CrudeMonteCarloSettingsWrapper.h"
#include "ReliabilityMethodWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Reliability/CrudeMonteCarlo.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			public ref class CrudeMonteCarloWrapper : public ReliabilityMethodWrapper
			{
			private:
				Reliability::CrudeMonteCarlo* m_crude_monte_carlo;
				SharedPointerProvider<Reliability::CrudeMonteCarlo>* sharedPointer = new SharedPointerProvider<Reliability::CrudeMonteCarlo>();
			public:
				CrudeMonteCarloWrapper()
				{
					m_crude_monte_carlo = new Reliability::CrudeMonteCarlo();
				}
				~CrudeMonteCarloWrapper() { this->!CrudeMonteCarloWrapper(); }
				!CrudeMonteCarloWrapper() { delete sharedPointer; }

				CrudeMonteCarloSettingsWrapper^ Settings = gcnew CrudeMonteCarloSettingsWrapper();

				std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
				{
					std::shared_ptr<Reliability::CrudeMonteCarlo> monteCarlo = sharedPointer->getSharedPointer(m_crude_monte_carlo);

					monteCarlo->Settings = Settings->GetSettings();
					return monteCarlo;
				}
			};
		}
	}
}



