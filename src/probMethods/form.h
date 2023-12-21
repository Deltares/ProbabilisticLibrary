#pragma once
#include "../utils/vector1D.h"
#include "lsf.h"
#include "zmodel.h"
#include "progress.h"
#include "dpoptions.h"

namespace Deltares {
    namespace ProbLibCore {

        struct FormSettings
        {
            double du = 0.01;
            int maxIter = 50;
            int trialLoops = 3; // only relevant for formWithTrialLoops
            double epsilonZvalue = 0.001;
            double epsilonBeta = 0.001;
            double relaxationFactor = 0.7;
            DPoptions designPointOption = DPoptions::None;
        };

        class form : public lsf
        {
        public:
            form(const FormSettings& s, const progress& p);
            form(const FormSettings& s, vector1D start, const progress& p);
            lsfResult calc(const zmodel& m, const stochSettings& s);
            const double betaMax = 40.0;
        private:
            const int histU = 10; // keep last 10 u values for history / statistics
            vector1D startVector;
            FormSettings settings;
            const progress& p;
            void estimateBetaNonConv(lsfResult& lsf, const std::vector<double>& allBetas, const std::vector<vector1D>& last10u);
        };
    }
}
