#pragma once
#include <utility>
#include <vector>
#include "CombineType.h"

namespace Deltares {
    namespace Reliability {

        class HohenbichlerFORM
        {
        public:
            std::pair<double, int> PerformHohenbichler(const double betaV, const double pfU, const double rhoInput);
        private:
            const int maxTrialLoops = 3;
            const double rhoLimitHohenbichler = 0.98;  // Limit value for the correlation coefficient
        };
    }
}
