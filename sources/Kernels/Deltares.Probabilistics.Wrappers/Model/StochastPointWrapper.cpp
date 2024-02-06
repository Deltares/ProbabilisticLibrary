#include "StochastPointWrapper.h"
#include "../Statistics/StochastWrapper.h"

#include <map>

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			void StochastPointWrapper::SetStochastPoint(std::shared_ptr<StochastPoint> stochastPoint, System::Collections::Generic::List<Statistics::Wrappers::StochastWrapper^>^ stochasts)
			{
				this->m_stochastPoint = stochastPoint.get();

				std::map<std::shared_ptr<Statistics::Stochast>, int> stochastMapping;

				for (int i = 0; i < stochasts->Count; i++)
				{
					stochastMapping[stochasts[i]->GetStochast()] = i;
				}

				for (int i = 0; i < stochastPoint->Alphas.size(); i++)
				{
					StochastPointAlphaWrapper^ alphaWrapper = gcnew StochastPointAlphaWrapper(stochastPoint->Alphas[i]);
					alphaWrapper->Parameter = stochasts[stochastMapping[stochastPoint->Alphas[i]->Stochast]];
					this->Alphas->Add(alphaWrapper);
				}
			}
		}
	}
}

