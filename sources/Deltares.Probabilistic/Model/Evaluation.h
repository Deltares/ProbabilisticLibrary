#pragma once

#include <cmath>

namespace Deltares
{
    namespace Models
    {
        class Evaluation
        {
        public:
            double Z = std::nan("");
            double Weight = 1;
            int Iteration = -1;
            int Tag = 0;
        };
    }
}

