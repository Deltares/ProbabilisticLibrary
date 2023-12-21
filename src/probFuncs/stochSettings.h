#pragma once
#include "../utils/matrix.h"
#include "../correlation/gaussianCorrelation.h"
#include "../distributions/distrib.h"
#include "../distributions/mtable.h"
#include "listDistribs.h"

namespace Deltares {
    namespace ProbLibCore {

        class stochSettings
        {
        public:
            stochSettings(const correlation& c, listDistribs& p);
            stochSettings(const correlation& c, listDistribs& p, const bool ip);
            void updateU(vector1D& u) const;
            size_t getNrStochasts() const;
            size_t getNrAllStochasts() const;
            std::pair<size_t, bool> getStochId(const size_t i) const;
            bool isFullyCorrelated(const int i) const { return corr.isFullyCorrelated(i); }
            vector1D applyCholesky(const vector1D& u) const { return corr.applyCholesky(u); }
            const distrib* getParamI(const size_t i) const;
            int getVectorSize() const { return params.getVectorSize(); };
        private:
            size_t calcNrStochasts();
            const correlation& corr;
            bool useIpoint;
            listDistribs& params;
            size_t nrStochasts;
        };
    }
}
