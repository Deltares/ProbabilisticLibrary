#pragma once

#include "DiscreteValue.h"
#include "HistogramValue.h"
#include "FragilityValue.h"

#include <vector>
#include <memory>

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

            std::vector<std::shared_ptr<DiscreteValue>> DiscreteValues;
            std::vector<std::shared_ptr<HistogramValue>> HistogramValues;
            std::vector<std::shared_ptr<FragilityValue>> FragilityValues;
        };

    }
}
