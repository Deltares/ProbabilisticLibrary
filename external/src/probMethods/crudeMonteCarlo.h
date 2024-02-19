#pragma once
#include "../rnd/randomForSampling.h"
#include "progress.h"
#include "lsf.h"

namespace Deltares {
    namespace ProbLibCore {

        struct CMsettings
        {
            int minimumSamples = 100000;
            int maximumSamples = 250000;
            double varCoeffNoFailure = 0.005;
            double varCoeffFailure = 0.005;
        };

        class crudeMonteCarlo : public lsf
        {
        public:
            crudeMonteCarlo(const CMsettings& s, randomForSampling& rnd, const progress& p);
            lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            CMsettings settings;
            randomForSampling& rnd;
            const progress& p;
        };
    }
}
