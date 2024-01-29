#pragma once

#include "DiscreteValue.h"
#include "HistogramValue.h"
#include "FragilityValue.h"

#include <vector>

namespace Deltares
{
    namespace Statistics
    {
        class StochastProperties
        {

        public:

            double Location;
            double Scale;
            double Minimum;
            double Maximum;
            double Shift;
            double ShiftB;
            double Shape;
            double ShapeB;
            int Observations;

            std::vector<DiscreteValue*> DiscreteValues;
            std::vector<HistogramValue*> HistogramValues;
            std::vector<FragilityValue*> FragilityValues;
        };

    }
}
