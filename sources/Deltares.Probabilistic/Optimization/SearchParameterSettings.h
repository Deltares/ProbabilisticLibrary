#pragma once
#include <cmath>
#include <vector>

#include "../Utils/probLibException.h"

namespace Deltares
{
	namespace Optimization
	{
		/**
		 * \brief Indicates which values have to be used from the search parameter settings
		 */
		enum UseValuesType {AllValues, MinValue, MaxValue};

		/**
		 * \brief Settings for a parameter in the grid search algorithm
		 */
		class SearchParameterSettings
		{
		public:
			/**
			 * \brief Minimum value which can be assigned to the parameter
			 */
			double MinValue = 0;

			/**
			 * \brief Maximum value which can be assigned to the parameter
			 */
			double MaxValue = 0;

			/**
			 * \brief Number of different values which can be assigned to the parameter
			 */
			int NumberOfValues = 1;

			/**
			 * \brief Start value for the parameter
			 * \remark Not used in the grid search algorithm, but in future algorithms
			 */
			double StartValue = nan("");

			/**
			 * \brief Indicates whether the grid can be repositioned for this parameter
			 * \remark Only useful when UseValues is AllValues
			 */
			bool Move = false;

			/**
			 * \brief The number of refinements to be performed
			 */
			int NumberOfRefinements = 0;

			/**
			 * \brief Indicates which values have to be used in the grid search algorithm
			 */
			UseValuesType UseValues = UseValuesType::AllValues;

			/**
			 * \brief Gets the values to be queried in the grid search algorithm 
			 * \return Values
			 * \remark Based on MinValue, MaxValue, UseValueType and NumberOfValues
			 */
			std::vector<double> getValues() const
			{
				std::vector<double> values(this->UseValues == UseValuesType::AllValues ? this->NumberOfValues : 1);

				if (this->UseValues == UseValuesType::AllValues) 
				{
					double interval = getInterval();

					for (int i = 0; i < values.size(); i++)
					{
						values[i] = MinValue + i * interval;
					}
				}
				else if (this->UseValues == UseValuesType::MinValue)
				{
					values[0] = MinValue;
				}
				else if (this->UseValues == UseValuesType::MaxValue)
				{
					values[0] = MaxValue;
				}
				else
				{
					throw Reliability::probLibException("Use values type not supported");
				}

				return values;
			}

			/**
			 * \brief Gets the interval between values
			 * \return Interval
			 */
			double getInterval() const
			{
				if (NumberOfValues > 1)
				{
					return (MaxValue - MinValue) / (NumberOfValues - 1);
				}
				else
				{
					return 0;
				}
			}
		};
	}
}

