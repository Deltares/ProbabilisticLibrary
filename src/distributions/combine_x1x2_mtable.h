#pragma once
#include "mtable.h"
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class combine_x1x2_mtable : public distrib
        {
        public:
            combine_x1x2_mtable(const mtable& tbl, const distrib4p& e, const size_t i, const size_t j);
            combine_x1x2_mtable(const mtable& tbl, const distrib4p& e);
            virtual double getx(const double x1, const double x2) const;
        private:
            const distrib4p& dist;
            const mtable* t;
        };
    }
}
