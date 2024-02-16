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
		class Sample
		{
		private:
			int size = 0;
			long tag = 0;

		public:
			Sample(int size)
			{
				this->size = size;
				for (size_t i = 0; i < size; i++)
				{
					Values.push_back(0.0);
				}
			}

			Sample(std::vector<double>& values)
			{
				this->size = (int)values.size();
				this->Values = values;
			}

			~Sample()
			{
			}

			std::vector<double> Values;

			int IterationIndex = -1;
			double Weight = 1;
			bool AllowProxy = true;
			double Z = nan("");

			int getSize();

			double getBeta();
			void setInitialValues(double beta);
			std::shared_ptr<Sample> clone();
			std::shared_ptr<Sample> getNormalizedSample() { return getSampleAtBeta(1); }
			std::shared_ptr<Sample> getSampleAtBeta(double beta);
			std::shared_ptr<Sample> getMultipliedSample(double factor);
			void correctSmallValues(double tolerance = 1E-10);

			intptr_t Tag = 0;
		};
	}
}

