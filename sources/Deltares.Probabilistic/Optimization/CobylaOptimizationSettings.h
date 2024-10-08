#pragma once
#include <vector>
namespace Deltares
{
    namespace Optimization
    {
        class CobylaOptimizationSettings
        {
        public:
            int MinIterations = 100;

            int MaxIterations = 10000;

            double RhoBegin = 0.5;

            double RhoEnd = 1.0e-6;
        };

    }
}

