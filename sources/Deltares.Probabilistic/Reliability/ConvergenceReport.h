#pragma once
#include <cmath>

namespace Deltares
{
    namespace Reliability
    {
        class ConvergenceReport
        {
        public:
            bool IsConverged = false;
            double Convergence = nan("");
            int FailedSamples = -1;
            double FailFraction = nan("");
            double RelaxationFactor = nan("");
            double VarianceFactor = nan("");
            double MaxWeight = nan("");
            double FailWeight = nan("");
            double ZMargin = 0;
            int TotalDirections = -1;
        };
    }
}

