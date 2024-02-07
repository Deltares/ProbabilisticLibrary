#pragma once

#include "CrudeMonteCarloSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/CrudeMonteCarlo.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			public ref class CrudeMonteCarlo : public ReliabilityMethod
			{
			private:
				Reliability::CrudeMonteCarlo* m_crude_monte_carlo;
				SharedPointerProvider<Reliability::CrudeMonteCarlo>* sharedPointer = new SharedPointerProvider<Reliability::CrudeMonteCarlo>();
			public:
				CrudeMonteCarlo()
				{
					m_crude_monte_carlo = new Reliability::CrudeMonteCarlo();
				}
				~CrudeMonteCarlo() { this->!CrudeMonteCarlo(); }
				!CrudeMonteCarlo() { delete sharedPointer; }

				CrudeMonteCarloSettings^ Settings = gcnew CrudeMonteCarloSettings();

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



