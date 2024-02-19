#pragma once
#include "lsf.h"
#include "form.h"
#include "dsfi.h"

namespace Deltares {
    namespace ProbLibCore {

        // class for running formDsfi:
        // start a FORM calculation;
        // if that fails, fall back on DSFI.
        class formDsfi : public lsf
        {
        public:
            formDsfi(const DSsettings& s1, const FormSettings& s2, randomForSampling& rnd, dsRootFinder& f, const progress& p);
            formDsfi(const DSsettings& s1, const FormSettings& s2, randomForSampling& rnd, dsRootFinder& f, const size_t nrCores, const progress& p);
            virtual lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            const DSsettings& DSFlags;
            const FormSettings& FormFlags;
            randomForSampling& rnd;
            const size_t numcores = 1;
            const progress& p;
            dsRootFinder& rtf;
        };

        // class for running formDsfiHR:
        // start a FORM calculation;
        // if that fails, fall back on DSFIHR.
        class formDsfiHR : public lsf
        {
        public:
            formDsfiHR(const DSsettings& s1, const FormSettings& s2, randomForSampling& rnd, dsRootFinder& f, const progress& p);
            formDsfiHR(const DSsettings& s1, const FormSettings& s2, randomForSampling& rnd, dsRootFinder& f, const size_t nrCores, const progress& p);
            virtual lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            const DSsettings& DSFlags;
            const FormSettings& FormFlags;
            randomForSampling& rnd;
            const size_t numcores = 1;
            const progress& p;
            dsRootFinder& rtf;
        };
    }
}
