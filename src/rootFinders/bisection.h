#pragma once
#include "root_finder.h"

namespace Deltares {
    namespace ProbLibCore {

        class bisection : public root_finder
        {
        public:
            bisection(FuncWrapper& f);
        protected:
            virtual double nextXvalue(const xyPair p0, const xyPair p1) const;
        };
    }
}
