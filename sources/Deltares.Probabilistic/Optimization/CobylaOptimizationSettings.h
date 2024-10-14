#pragma once

namespace Deltares
{
    namespace Optimization
    {
        class CobylaOptimizationSettings
        {
        public:
            int MaxIterations = 10000;

            double EpsilonBeta = 0.001;
        };

    }
}

