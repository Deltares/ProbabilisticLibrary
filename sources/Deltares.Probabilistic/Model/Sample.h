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
		/// <summary>
		/// Indicates the meaning of <see cref="Sample.Values"/> in a <see cref="Sample"/>
		/// </summary>
		enum SpaceType { U, X };

		class Sample
		{
		private:
			int size = 0;

		public:
			Sample(int size)
			{
				this->size = size;
				for (int i = 0; i < size; i++)
				{
					Values.push_back(0.0);
				}
			}

			Sample(std::vector<double> values)
			{
				this->size = (int)values.size();
				this->Values = values;
			}

			~Sample()
			{
			}

			std::vector<double> Values;

			/**
			 * \brief Defines the parameter space in which Values are defined
			 */
			SpaceType spaceType = SpaceType::U;

			int IterationIndex = -1;
			double Weight = nan("");
			bool AllowProxy = true;
			bool IsRestartRequired = false;
			double Z = nan("");

			int getSize();

			double getBeta();
			double getDistance(std::shared_ptr<Sample> other);
			double getDistance2(std::shared_ptr<Sample> other);
			void setInitialValues(double beta);
			std::shared_ptr<Sample> clone();
			std::shared_ptr<Sample> getNormalizedSample() { return getSampleAtBeta(1); }
			std::shared_ptr<Sample> getSampleAtBeta(double beta);
			std::shared_ptr<Sample> getMultipliedSample(double factor);
			void correctSmallValues(double tolerance = 1E-10);
			bool areValuesEqual(std::shared_ptr<Sample> other);
		};
	}
}

