#pragma once
#include "root_finder.h"

namespace Deltares {
    namespace ProbLibCore {

        class Anderson_Bjork : public root_finder
        {
        public:
            Anderson_Bjork(FuncWrapper& f);
            Anderson_Bjork(FuncWrapper& f, const double e, const size_t mxi);
        private:
            virtual double nextXvalue(const xyPair p0, const xyPair p1) const;
            double nextValueAB(const xyPair p0, const xyPair p1) const;
        };
    }
}
