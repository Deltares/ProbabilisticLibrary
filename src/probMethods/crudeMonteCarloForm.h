#pragma once
#include "lsf.h"
#include "form.h"
#include "crudeMonteCarlo.h"

namespace Deltares {
    namespace ProbLibCore {

        // class for running Crude Monte Carlo with FORM iterations:
        // start a CM calculation;
        // after that, run Form to improve alpha and x values.
        class crudeMonteCarloForm : public lsf
        {
        public:
            crudeMonteCarloForm(const CMsettings& s1, const FormSettings& s2, randomForSampling& rnd, const progress& p);
            crudeMonteCarloForm(const CMsettings& s1, const FormSettings& s2, randomForSampling& rnd, const size_t nrCores, const progress& p);
            virtual lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            const CMsettings& CMFlags;
            const FormSettings& FormFlags;
            randomForSampling& rnd;
            const size_t numcores = 1;
            const progress& p;
        };
    }
}
