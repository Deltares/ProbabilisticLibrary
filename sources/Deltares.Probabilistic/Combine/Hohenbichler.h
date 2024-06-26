#pragma once
#include <utility>
#include <vector>
#include "CombineType.h"

namespace Deltares {
    namespace Reliability {

        class Hohenbichler
        {
        public:
            std::pair<double, int> PerformHohenbichler(const double betaV, const double pfU, const double rhoInput);
            bool useNumInt = false;
        private:
            double BetaHohenbichler(double dp1, double dp2, double rho, combineAndOr system);
            double HohenbichlerNumInt(double dp1, double dp2, double rho, combineAndOr system);
            std::vector<double> LinearSpaced(int length, double start, double stop);
            const int maxTrialLoops = 3;
            const double rhoLimitHohenbichler = 0.98;  // Limit value for the correlation coefficient
        };
    }
}
