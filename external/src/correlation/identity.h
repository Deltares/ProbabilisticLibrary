#pragma once
#include "correlation.h"

namespace Deltares {
    namespace ProbLibCore {

        class identity : public correlation
        {
        public:
            identity(const int nrStochs);
            virtual ~identity() = default;
            void virtual updateU(vector1D& u) const;
            int virtual cntIndependent() const;
            bool virtual isFullyCorrelated(const int i) const { return false; }
            vector1D applyCholesky(const vector1D& u) const { return u; }
        private:
            const int nrStochasts;
        };
    }
}
