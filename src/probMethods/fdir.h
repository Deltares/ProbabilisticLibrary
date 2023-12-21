#pragma once
#include "form.h"
#include "directionalSampling.h"

namespace Deltares {
    namespace ProbLibCore {

        // class for running fdir:
        // start a FORM calculation;
        // if that does not converges, or the beta is not in range [FDthreshold, betaMax],
        // run Directional Sampling.
        class fdir : public lsf
        {
        public:
            fdir(DSsettings s1, FormSettings s2, randomForSampling& rnd, dsRootFinder& f, const progress& p);
            fdir(DSsettings s1, FormSettings s2, randomForSampling& rnd, dsRootFinder& f, vector1D& u, const progress& p);
            fdir(DSsettings s1, FormSettings s2, randomForSampling& rnd, dsRootFinder& f, const size_t nrCores, const progress& p);
            fdir(DSsettings s1, FormSettings s2, randomForSampling& rnd, dsRootFinder& f, vector1D& u, const size_t nrCores, const progress& p);
            virtual lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            bool acceptableBeta(const double beta);
            FormSettings FormFlags;
            DSsettings   DSFlags;
            randomForSampling& rnd;
            const size_t numcores = 1;
            vector1D startVector = vector1D(0);
            const progress& p;
            dsRootFinder& rtf;
            const double betaMax = 40.0;
            const double FDthreshold = -2.0;
        };
    }
}
