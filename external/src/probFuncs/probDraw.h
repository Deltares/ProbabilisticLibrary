#pragma once
#include "../utils/vector1D.h"
#include "stochSettings.h"

namespace Deltares {
    namespace ProbLibCore {

        class probDraw
        {
        public:
            probDraw(const size_t n, const double x, const stochSettings& s);
            probDraw(const size_t n, const vector1D& x, const stochSettings& s);
            double getx(size_t i) const;
            void setu(const vector1D& u);
        private:
            vector1D u;
            void init_corr(const stochSettings& s);
            const stochSettings& stochParams;
        };
    }
}
