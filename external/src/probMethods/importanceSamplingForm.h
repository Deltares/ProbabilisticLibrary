#pragma once
#include "lsf.h"
#include "form.h"
#include "importanceSampling.h"

namespace Deltares {
    namespace ProbLibCore {

        // class for running Monte Carlo Importance Sampling with FORM iterations:
        // start a CMIM calculation;
        // after that, run Form to improve alpha and x values.
        class importanceSamplingForm : public lsf
        {
        public:
            importanceSamplingForm(const IMsettings& s1, const FormSettings& s2, randomForSampling& rnd, const progress& p);
            importanceSamplingForm(const IMsettings& s1, const FormSettings& s2, randomForSampling& rnd, const size_t nrCores, const progress& p);
            virtual lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            const IMsettings& IMFlags;
            const FormSettings& FormFlags;
            randomForSampling& rnd;
            const size_t numcores = 1;
            const progress& p;
        };
    }
}
