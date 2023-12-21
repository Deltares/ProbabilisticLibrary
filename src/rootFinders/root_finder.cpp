#include <cmath>
#include "root_finder.h"
#include "../utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

        root_finder::root_finder(FuncWrapper& f, const double e, const size_t mxi) :
            func(f), epsX(e), epsY(e), maxIter(mxi)
        {
            checkParam(e);
        };

        root_finder::root_finder(FuncWrapper& f, const double e, const size_t mxi, const bool inBetween) :
            func(f), epsX(e), epsY(e), maxIter(mxi), mustBeInbetween(inBetween)
        {
            checkParam(e);
        };

        void root_finder::checkParam(const double e)
        {
            if (e < 0.0) throw probLibException("eps in root_finder must be >= 0.0");
        };

        std::pair<bool, double> root_finder::FindRoot(const double x0, const double x1)
        {
            state = std::vector<xyPair>();
            for (size_t i = 0; i < 2; i++)
            {
                double x = (i == 0 ? x0 : x1);
                state.push_back({ x, func.GetValue(x) });
                if (fabs(state[i].y) < epsY)
                {
                    return { true, state[i].x };
                }
            }
            if (mustBeInbetween && state[0].y * state[1].y > 0.0)
            {
                return { false, -999.0 };
            }
            size_t i0 = 0;
            size_t i1 = 1;
            bool success = false;
            for (size_t i = 0; i < maxIter; i++)
            {
                xyPair p2;
                if (fabs(state[i0].x - state[i1].x) < epsX)
                {
                    success = true;
                    break;
                }
                p2.x = nextXvalue(state[i0], state[i1]);
                p2.y = func.GetValue(p2.x);
                state.push_back(p2);
                if (fabs(p2.y) < epsY)
                {
                    success = true;
                    break;
                }
                else if (state[i0].y * state[i1].y > 0.0)
                {
                    if (fabs(state[i0].y) > fabs(state[i1].y))
                    {
                        i0 = 2 + i;
                        std::swap(i0, i1);
                    }
                    else
                    {
                        i1 = 2 + i;
                    }
                }
                else if (p2.y * state[i1].y > 0.0)
                {
                    i1 = 2 + i;
                }
                else
                {
                    i0 = 2 + i;
                }
            }

            return { success, state[state.size() - 1].x };
        }

        void root_finder::getStats(double& y, size_t& steps) const
        {
            steps = state.size();
            y = state[steps - 1].y;
        }
    }
}
