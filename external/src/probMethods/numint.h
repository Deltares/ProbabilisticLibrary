#pragma once
#include "lsf.h"
#include "progress.h"
#include "dpoptions.h"

namespace Deltares {
    namespace ProbLibCore {

        struct NIsettings
        {
            int numExtraInt = 500;
            double minimumUvalue = -3.0;
            double maximumUvalue = 3.0;
            DPoptions designPointOption = DPoptions::None;
        };

        struct NIstate
        {
            double z0Fac;
            double density;
            double rBeta;
            double rMin;
            double probFailure;
            size_t i;
            int threadId;
        };

        class numint : public lsf
        {
        public:
            numint(const NIsettings& s, const progress& p);
            numint(const NIsettings& s, const size_t nrProc, const progress& p);
            lsfResult calc(const zmodel& mdl, const stochSettings& s);
        private:
            void recursiveNumInt(vector1D& drw, lsfResult& result, const stochSettings& s, NIstate& state);
            void init_weights();

            const NIsettings& settings;
            const zmodel* m;
            std::vector<std::pair<double, double>> diffpq;
            const size_t numThreads = 1;
            const progress& p;
        };
    }
}
