#pragma once
#include <vector>

#include "DistributionType.h"
#include "VariableStochastValue.h"
#include "Distributions/Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class VariableStochastValuesSet
        {
        public:
            std::vector<std::shared_ptr<VariableStochastValue>> StochastValues;
            void initializeForRun();
            std::shared_ptr<StochastProperties> getInterpolatedStochast(double x);
            bool isVarying(DistributionType distributionType);
            void copyFrom(std::shared_ptr<VariableStochastValuesSet> source);
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
        };
    }
}

