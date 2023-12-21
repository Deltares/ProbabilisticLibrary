#pragma once
#include "../rnd/randomForSampling.h"
#include "lsf.h"
#include "progress.h"

namespace Deltares {
    namespace ProbLibCore {

        struct IMsettings
        {
            double varCoeffNoFailure = 0.1;
            double varCoeffFailure = 0.1;
            double varianceFactor = 1.5;
            vector1D varianceFactors;
            vector1D offsets;
            int minimumSamples = 1000;
            int maximumSamples = 100000;
            int chunkSize = 200;
            bool restart = false;
        };

        class ImportanceSamplingSeq : public lsf
        {
        public:
            ImportanceSamplingSeq(const IMsettings& s, randomForSampling& rnd, const progress& p);
            lsfResult calc(const zmodel& m, const stochSettings& s);
            void updateMaxSamples(const int maxSamples);
            void updateVariance(const double additionalVariance);
            void updateOffset(const vector1D& newOffset);
        private:
            IMsettings settings;
            randomForSampling& rnd;
            const progress& p;
        };
    }
}
