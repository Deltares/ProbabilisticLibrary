#include "Anderson_Bjork.h"

namespace Deltares {
    namespace ProbLibCore {

        Anderson_Bjork::Anderson_Bjork(FuncWrapper& f) : root_finder(f) {}

        Anderson_Bjork::Anderson_Bjork(FuncWrapper& f, const double e, const size_t mxi) :
            root_finder(f, e, mxi) {};

        double Anderson_Bjork::nextXvalue(const xyPair p0, const xyPair p1) const
        {
            if (state.size() < 3)
            {
                // start with one Regula-Falsi step
                return p0.x - p0.y / (p1.y - p0.y) * (p1.x - p0.x);
            }
            else
            {
                return nextValueAB(p0, p1);
            }
        }

        double Anderson_Bjork::nextValueAB(const xyPair p0, const xyPair p1) const
        {
            auto last = state.size() - 1;
            bool signCheck1 = state[last - 1].y * state[last].y > 0.0;
            bool useRegulaFalsi = !signCheck1;
            double f1 = p0.y;
            double f2 = p1.y;
            double f3 = state[last].y;
            double x1 = p0.x;
            double x2 = p1.x;
            double x3 = state[last].x;
            double phi;
            if (useRegulaFalsi)
            {
                phi = 1.0;
            }
            else // Anderson / Bjork:
            {
                phi = 1.0 - state[last].y / state[last - 1].y;
                if (phi < 0.0) phi = 0.5;
            }
            bool signCheck2 = f2 * f3 > 0.0;
            if (signCheck2)
            {
                f1 = phi * f1;
                return (f3 * x1 - f1 * x3) / (f3 - f1);
            }
            else
            {
                f2 = phi * f2;
                return (f3 * x2 - f2 * x3) / (f3 - f2);
            }
        }
    }
}
