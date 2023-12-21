#pragma once
#include <vector>
#include "correlation.h"
#include "../utils/matrix.h"
#include "gaussianCorrelation.h"

namespace Deltares {
    namespace ProbLibCore {

        class gaussianCorrelationRobust : public gaussianCorrelation
        {
        public:
            ~gaussianCorrelationRobust() {};
            gaussianCorrelationRobust(const int n, const std::vector<corrStruct>& pairs);
            bool virtual isFullyCorrelated(const int i) const;
            vector1D applyCholesky(const vector1D& u) const { return cholesky.matvec(u); }
        private:
            void updateCorrelationMatrixRobust(const size_t nStochasts, const std::vector<corrStruct>& pairs);
        };
    }
}
