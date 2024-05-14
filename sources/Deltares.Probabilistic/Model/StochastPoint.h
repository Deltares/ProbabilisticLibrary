#pragma once

#include <vector>

#include "Sample.h"
#include "ModelSample.h"
#include "StochastPointAlpha.h"

namespace Deltares
{
	namespace Models
	{
		class StochastPoint
		{
		public:
			double Beta;
			std::vector<std::shared_ptr<StochastPointAlpha>> Alphas;

			std::shared_ptr<Sample> getSample();
            std::shared_ptr<ModelSample> getModelSample();

			void updateInfluenceFactors();

			virtual ~StochastPoint() { ; }
		};
	}
}

