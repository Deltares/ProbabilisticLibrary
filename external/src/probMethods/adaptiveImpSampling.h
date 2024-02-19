#pragma once
#include "importanceSampling.h"
#include "fillStartVector.h"

namespace Deltares {
    namespace ProbLibCore {

        struct AIMsettings : public IMsettings
        {
            double       epsFailed = 1E-2;
            double       increaseVariance = 0.5;
            StartMethods startMethod;
            int          nAdp;
            int          maxSamplesDef;
            int          minFailed = 5;
            vector1D     startVector;
        };

        class adaptiveImpSampling : public lsf
        {
        public:
            adaptiveImpSampling(const AIMsettings& s, randomForSampling& rnd, const progress& p, const bool compatible);
            lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            lsfResult loopMCIS(const zmodel& m, const stochSettings& s, const vector1D& u);
            void printProgress(const int i, const lsfResult& result);
            AIMsettings settings;
            randomForSampling& rnd;
            const progress& p;
            const bool beCompatible;
        };
    }
}
