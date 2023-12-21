#pragma once
#include "../probFuncs/probDraw.h"

namespace Deltares {
    namespace ProbLibCore {

        enum class OutputFlags
        {
            designPointOutputTRUE = 0,
            designPointOutputFALSE = 1,
            designPointOutputPrintAll = 3,
        };

        class zmodel
        {
        public:
            zmodel() : numcores(1) {};
            zmodel(const size_t nrThreads) : numcores(nrThreads) {};
            virtual double zfunc(const probDraw& d) const = 0;
            virtual double zfunc(const probDraw& d, const int threadId) const { return zfunc(d); };
            virtual double zfunc(const probDraw& d, const OutputFlags option, vector1D& x) const { return -999.0; }
            virtual void getZandDerivatives(const vector1D& drwu, const stochSettings& s,
                double& z, vector1D& dzdu, const double du) const;
            virtual void logInfo(const int id) const {};
        private:
            void getZandDerivatives_seq(const vector1D& drwu, const stochSettings& s,
                double& z, vector1D& dzdu, const double du) const;
            void getZandDerivatives_omp(const vector1D& drwu, const stochSettings& s,
                double& z, vector1D& dzdu, const double du) const;
        public:
            const size_t numcores;
        };
    }
}
