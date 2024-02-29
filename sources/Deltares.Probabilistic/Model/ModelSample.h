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
			ModelSample(std::vector<double> values, std::vector<double> uValues)
			{
				this->Values = values;
				this->UValues = uValues;
			}

			std::vector<double> Values;
			std::vector<double> UValues;

			int IterationIndex = -1;
			double Weight = 1;
			bool AllowProxy = true;
			double Z = nan("");

			intptr_t Tag = 0;
		};
	}
}

