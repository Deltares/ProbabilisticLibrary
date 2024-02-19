#include "normalDist.h"

namespace Deltares {
    namespace ProbLibCore {

        double normalDist::getValue(const double u) const
        {
            return a + u * b;
        }

        double uSpace::getValue(const double u) const
        {
            return u;
        }

        double normalDist4p::getValue(const double u, const double p1, const double p2,
            const double p3, const double p4) const
        {
            auto d = normalDist(p1, p2);
            return d.getValue(u);
        }
    }
}
