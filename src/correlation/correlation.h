#pragma once
#include "../utils/vector1D.h"

namespace Deltares {
    namespace ProbLibCore {

        class correlation
        {
        public:
            virtual ~correlation() {};
            void virtual updateU(vector1D& u) const = 0;
            int virtual cntIndependent() const = 0;
            bool virtual isFullyCorrelated(const int i) const = 0;
            vector1D virtual applyCholesky(const vector1D& u) const = 0;
        };
    }
}
