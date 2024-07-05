#include "StochastPoint.h"
#include "../Statistics/Stochast.h"

#include <map>

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			void StochastPoint::setStochastPoint(std::shared_ptr<Models::StochastPoint> stochastPoint, System::Collections::Generic::IList<Statistics::Wrappers::Stochast^>^ stochasts)
			{
				shared = new SharedPointerProvider(stochastPoint);

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

