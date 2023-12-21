#pragma once
#include "form.h"
#include "directionalSampling.h"

namespace Deltares {
    namespace ProbLibCore {

        // class for running dsfi:
        // start a DS calculation;
        // after that, run Form to improve alpha values.
        class dsfi : public lsf
        {
        public:
            dsfi(DSsettings s1, FormSettings s2, randomForSampling& rnd, dsRootFinder& f, const progress& p);
            dsfi(DSsettings s1, FormSettings s2, randomForSampling& rnd, dsRootFinder& f, const size_t nrCores, const progress& p);
            virtual lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            FormSettings FormFlags;
            DSsettings   DSFlags;
            randomForSampling& rnd;
            const size_t numcores = 1;
            const progress& p;
            dsRootFinder& rtf;
        };

        // class for running dsfiHR:
        // start a DS calculation;
        // after that, run Form to improve alpha values.
        class dsfiHR : public lsf
        {
        public:
            dsfiHR(DSsettings s1, FormSettings s2, randomForSampling& rnd, dsRootFinder& f, const progress& p);
            dsfiHR(DSsettings s1, FormSettings s2, randomForSampling& rnd, dsRootFinder& f, const size_t nrCores, const progress& p);
            virtual lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            FormSettings FormFlags;
            DSsettings   DSFlags;
            randomForSampling& rnd;
            const size_t numcores = 1;
            const progress& p;
            dsRootFinder& rtf;
        };
    }
}
