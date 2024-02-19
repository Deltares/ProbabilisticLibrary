#pragma once
#include "../rnd/randomForSampling.h"
#include "lsf.h"
#include "progress.h"
#include "importanceSamplingSeq.h"

namespace Deltares {
    namespace ProbLibCore {

        class IMbuffer
        {
        public:
            IMbuffer(const size_t bufSize, const size_t nrStochasts, const bool circular);
            bool circular = true;
            int lastFilled = 0;
            vector1D zpar;
            vector1D dPfPar;
            vector1D fTotalPar;
            std::vector<vector1D> drwPar2;
        };

        class ImportanceSampling : public lsf
        {
        public:
            ImportanceSampling(const IMsettings& s, randomForSampling& rnd, const progress& p, IMbuffer& bffr);
            lsfResult calc(const zmodel& m, const stochSettings& s);
            void updateMaxSamples(const int maxSamples, const bool restart);
            void updateVariance(const double additionalVariance);
            void updateOffset(const vector1D& newOffset);
        private:
            void updateBuffer(const zmodel& m, const stochSettings& s, const int nTimes, const int offset);
            IMsettings settings;
            randomForSampling& rnd;
            const progress& p;
            IMbuffer& buf;
            vector1D offsets;
            vector1D varianceFactors;
            double varianceFactorPowN;
            int bufferSize;
        };
    }
}
