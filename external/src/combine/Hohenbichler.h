#pragma once
#include "../probMethods/zmodel.h"
#include "../probMethods/formWithTrialLoops.h"
#include "../correlation/identity.h"

namespace Deltares {
    namespace ProbLibCore {

        class HohenbichlerZ : public zmodel
        {
        public:
            HohenbichlerZ(const double betaV, const double pfU, const double r);
            virtual double zfunc(const probDraw& d) const;
        private:
            const double beta2;
            const double pf1;
            const double rho;
            const double squaredRoot;
        };

        class Hohenbichler
        {
        public:
            Hohenbichler(const progress& p);
            ~Hohenbichler();
            std::pair<double, ConvergenceStatus> PerformHohenbichler(const double betaV, const double pfU, const double rhoInput);
        private:
            const int maxTrialLoops = 3;
            const double rhoLimitHohenbichler = 0.98;  // Limit value for the correlation coefficient
            formWithTrialLoops* Form;
            listDistribs params;
            identity id = identity(2);
            const progress& p;
        };
    }
}
