#pragma once

#include "../Model/ZModel.h"

namespace Deltares {
    namespace Reliability {

        class HohenbichlerZ
        {
        public:
            HohenbichlerZ(const double betaV, const double pfU, const double r);
            double zfunc(std::vector<double>& x) const;
        private:
            const double beta2;
            const double pf1;
            const double rho;
            const double squaredRoot;
        };

        class Hohenbichler
        {
        public:
            std::pair<double, int> PerformHohenbichler(const double betaV, const double pfU, const double rhoInput);
        private:
            const int maxTrialLoops = 3;
            const double rhoLimitHohenbichler = 0.98;  // Limit value for the correlation coefficient
        };
    }
}
