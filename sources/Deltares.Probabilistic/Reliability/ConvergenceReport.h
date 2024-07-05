#pragma once
#include <cmath>

#include "../Model/Sample.h"

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
            int TotalIterations = -1;

            std::shared_ptr<Models::Sample> NearestSample = nullptr;

            double getSmallestFraction()
            {
                return std::min(FailFraction, 1 - FailFraction);
            }
        };
    }
}

