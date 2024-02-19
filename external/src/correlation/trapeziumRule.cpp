#include "trapeziumRule.h"
#include <cstddef>

namespace Deltares {
    namespace ProbLibCore {

        double trapeziumRule::calc(const std::vector<xyPair>& xy)
        {
            double firstStepSize = xy[1].x - xy[0].x;
            double trapeziumRule = xy[0].y * firstStepSize;

            for (size_t ix = 1; ix < xy.size() - 1; ix++)
            {
                double stepSize = xy[ix + 1].x - xy[ix - 1].x;
                trapeziumRule += xy[ix].y * stepSize;
            }

            double lastStepSize = xy[xy.size() - 1].x - xy[xy.size() - 2].x;
            trapeziumRule += xy[xy.size() - 1].y * lastStepSize;

            // the algorithm above calculates twice the integral, so correct that:
            return trapeziumRule * 0.5;
        }
    }
}
