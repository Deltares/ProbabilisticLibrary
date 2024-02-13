#pragma once
#include <vector>

#include "VariableStochastValue.h"
#include "Distributions/Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		class VariableStochastValuesSet
		{
		private:
			std::vector<double> xValues;
			std::vector<double> locations;
			std::vector<double> scales;
			std::vector<double> minimums;
			std::vector<double> maximums;
			std::vector<double> shapes;
			std::vector<double> shapesB;
			std::vector<double> shifts;
			std::vector<double> shiftsB;
			std::vector<double> observations;

			std::shared_ptr<Distribution> distribution = nullptr;
		public:
			std::vector<std::shared_ptr<VariableStochastValue>> StochastValues;
			void initializeForRun();
			void setDistribution(std::shared_ptr<Distribution> distribution) { this->distribution = distribution; }
			std::shared_ptr<StochastProperties> getInterpolatedStochast(double x);
			bool isVarying();
		};
	}
}

