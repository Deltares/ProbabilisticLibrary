#pragma once
#include <cmath>
#include <memory>

namespace Deltares
{
    namespace Reliability
    {
        class ReliabilityResult
        {
        public:
            int ProxyIteration = -1;
            int Run = 0;
            int Index = -1;
            double Reliability = nan("");
            double LowReliability = nan("");
            double HighReliability = nan("");
            double Samples = nan("");
            double ConvBeta = nan("");
            double ProxyConvergence = nan("");
            double Variation = nan("");
            double Difference = nan("");
            double Contribution = nan("");

            bool IsMeaningful(std::shared_ptr<ReliabilityResult> resultBefore, std::shared_ptr<ReliabilityResult> resultAfter);

        };
    }
}

