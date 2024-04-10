#pragma once
#include <cmath>
#include <vector>

#include "../Utils/probLibException.h"

namespace Deltares
{
	namespace Optimization
	{
		enum UseValuesType {AllValues, MinValue, MaxValue};

		class SearchParameterSettings
		{
		public:
			double MinValue = 0;
			double MaxValue = 0;
			int NumberOfValues = 1;
			double StartValue = nan("");
			bool Move = false;
			int NumberOfRefinements = 0;

			UseValuesType UseValues = UseValuesType::AllValues;

			std::vector<double> getValues()
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

			double getInterval()
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

