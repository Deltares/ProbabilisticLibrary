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

            double Location = 0;
            double Scale = 0;
            double Minimum = 0;
            double Maximum = 0;
            double Shift = 0;
            double ShiftB = 0;
            double Shape = 1;
            double ShapeB = 1;
            int Observations = 2;

            std::vector<std::shared_ptr<DiscreteValue>> DiscreteValues;
            std::vector<std::shared_ptr<HistogramValue>> HistogramValues;
            std::vector<std::shared_ptr<FragilityValue>> FragilityValues;
        };

    }
}
