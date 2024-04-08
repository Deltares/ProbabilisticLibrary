#pragma once
#include <cmath>
#include <vector>

namespace Deltares
{
	namespace Optimization
	{
		class SearchParameterSettings
		{
		public:
			double MinValue = 0;
			double MaxValue = 0;
			int NumberOfValues = 1;
			double StartValue = nan("");
			bool Move = false;
			int NumberOfRefinements = 0;
			double Interval = 0;

			std::vector<double> EnumeratedValues;

			std::vector<double> getValues()
			{
				std::vector<double> values(this->NumberOfValues);
				double interval = (MaxValue - MinValue) / (NumberOfValues - 1);

				for (int i = 0; i < values.size(); i++)
				{
					values[i] = MinValue + i * interval;
				}

				return values;
			}
		};
	}
}

