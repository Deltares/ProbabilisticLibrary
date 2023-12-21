#pragma once
#include "root_finder.h"

namespace Deltares {
    namespace ProbLibCore {

        class regula_falsi : public root_finder
        {
        public:
            regula_falsi(FuncWrapper& f);
            regula_falsi(FuncWrapper& f, const double e, const size_t mxi);
        protected:
            virtual double nextXvalue(const xyPair p0, const xyPair p1) const;
        };
    }
}
