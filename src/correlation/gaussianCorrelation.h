#pragma once
#include <vector>
#include "correlation.h"
#include "../utils/matrix.h"

namespace Deltares {
    namespace ProbLibCore {

        struct corrStruct
        {
            int idx1;
            int idx2;
            double correlation;
        };

        class gaussianCorrelation : public correlation
        {
        public:
            gaussianCorrelation(const int n) : cholesky(n, n) {};
            gaussianCorrelation(const int n, const std::vector<corrStruct>& pairs);
            ~gaussianCorrelation() {};
            void virtual updateU(vector1D& u) const;
            int virtual cntIndependent() const;
            bool virtual isFullyCorrelated(const int i) const;
            vector1D applyCholesky(const vector1D& u) const { return cholesky.matvec(u); }
        private:
            void updateCorrelationMatrix(const size_t nStochasts, const std::vector<corrStruct>& pairs);
        protected:
            int independent = -999;
            Matrix cholesky;
        };
    }
}
