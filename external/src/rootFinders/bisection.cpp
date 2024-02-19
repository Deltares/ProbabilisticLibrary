#include "bisection.h"

namespace Deltares {
    namespace ProbLibCore {

        bisection::bisection(FuncWrapper& f) : root_finder(f, true) {}

        double bisection::nextXvalue(const xyPair p0, const xyPair p1) const
        {
            return 0.5 * (p0.x + p1.x);;
        }
    }
}
