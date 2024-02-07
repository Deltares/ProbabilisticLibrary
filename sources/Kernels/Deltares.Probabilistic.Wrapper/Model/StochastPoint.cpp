#include "StochastPoint.h"
#include "../Statistics/Stochast.h"

#include <map>

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			void StochastPoint::SetStochastPoint(std::shared_ptr<Models::StochastPoint> stochastPoint, System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts)
			{
				this->m_stochastPoint = stochastPoint.get();

				std::map<std::shared_ptr<Statistics::Stochast>, int> stochastMapping;

				for (int i = 0; i < stochasts->Count; i++)
				{
					stochastMapping[stochasts[i]->GetStochast()] = i;
				}

				for (int i = 0; i < stochastPoint->Alphas.size(); i++)
				{
					StochastPointAlpha^ alphaWrapper = gcnew StochastPointAlpha(stochastPoint->Alphas[i]);
					alphaWrapper->Parameter = stochasts[stochastMapping[stochastPoint->Alphas[i]->Stochast]];
					this->Alphas->Add(alphaWrapper);
				}
			}
		}
	}
}

