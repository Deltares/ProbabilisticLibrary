#pragma once

#include <cmath>
#include <vector>
#include <cstdint>
#include <memory>

#include "../Math/NumericSupport.h"


namespace Deltares
{
	namespace Models
	{
		class ModelSample
		{
		private:
			long tag = 0;

		public:
			ModelSample(std::vector<double> values)
			{
				this->Values = values;
			}

			std::vector<double> Values;

			int IterationIndex = -1;
			double Weight = 1;
			bool AllowProxy = true;
			bool IsRestartRequired = false;
			double Beta = 0;
			double Z = nan("");

			intptr_t Tag = 0;
		};
	}
}

