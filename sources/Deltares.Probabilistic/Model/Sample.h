#pragma once

#include <cmath>
#include <vector>
#include <memory>

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

            /**
             * \brief Resets all contents of the sample to its default values
             */
            void clear()
			{
			    for (size_t i = 0; i < Values.size(); i++)
			    {
                    Values[i] = 0;
			    }

                IterationIndex = -1;
                threadId = 0;
                Weight = std::nan("");
                AllowProxy = true;
                IsRestartRequired = false;
                Z = std::nan("");
			}

			std::vector<double> Values;

			/**
			 * \brief Defines the parameter space in which Values are defined
			 */
			SpaceType spaceType = SpaceType::U;

			int IterationIndex = -1;
			int threadId = 0;
			double Weight = std::nan("");
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

