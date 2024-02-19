#pragma once
#include "dsSettings.h"
#include "dsRootFinder.h"
#include "../rnd/randomForSampling.h"
#include "../utils/vector1D.h"
#include "lsf.h"
#include "zmodel.h"
#include "progress.h"

namespace Deltares {
    namespace ProbLibCore {

        class directionalSampling : public lsf
        {
        public:
            directionalSampling(const DSsettings& s, randomForSampling& rnd, dsRootFinder& f, const progress& p);
            directionalSampling(const DSsettings& s, randomForSampling& rnd, dsRootFinder& f, const size_t numThreads,
                const progress& p);
            lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            void sampling_loop_seq(const zmodel& m, const stochSettings& s, lsfResult& lsf, const double z0);
            void sampling_loop_par(const zmodel& m, const stochSettings& s, lsfResult& lsf, const double z0);
            bool checkConvergence(const double dTimes, const double Pf, const double sumPfSamp,
                const double sumPfSamp2, double& convIndicator);
            void final_result(lsfResult& lsf, const double z0, const double Pf, const double rMin);
            const DSsettings& settings;
            randomForSampling& rnd;
            const size_t numcores = 1;
            const progress& pg;
            dsRootFinder& rootFinder;
        };
    }
}
