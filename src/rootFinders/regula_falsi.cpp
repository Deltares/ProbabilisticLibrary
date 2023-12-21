#include "regula_falsi.h"

namespace Deltares {
    namespace ProbLibCore {

        regula_falsi::regula_falsi(FuncWrapper& f) : root_finder(f) {}

        regula_falsi::regula_falsi(FuncWrapper& f, const double e, const size_t mxi) :
            root_finder(f, e, mxi) {}

        double regula_falsi::nextXvalue(const xyPair p0, const xyPair p1) const
        {
            return p1.x - p1.y * (p1.x - p0.x) / (p1.y - p0.y);
        }
    }
}
