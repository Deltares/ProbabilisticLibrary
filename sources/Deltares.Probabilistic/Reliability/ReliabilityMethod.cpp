#include "ReliabilityMethod.h"

#include <cmath>

namespace Deltares
{
    namespace Reliability
    {
        int ReliabilityMethod::getZFactor(double z)
        {
            return std::isnan(z) || z >= 0.0 ? 1 : -1;
        }

        bool ReliabilityMethod::isStopped()
        {
            return this->stopped;
        }

        void ReliabilityMethod::setStopped()
        {
            this->stopped = true;
        }

        void ReliabilityMethod::Stop()
        {
            setStopped();
        }
    }
}


