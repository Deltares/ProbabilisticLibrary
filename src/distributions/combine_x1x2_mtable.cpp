#include "combine_x1x2_mtable.h"
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        combine_x1x2_mtable::combine_x1x2_mtable(const mtable& tbl, const distrib4p& e, const size_t i, const size_t j)
            : distrib(i, j), dist(e)
        {
            t = &tbl;
        }

        combine_x1x2_mtable::combine_x1x2_mtable(const mtable& tbl, const distrib4p& e) : distrib((size_t)0, (size_t)0), dist(e)
        {
            t = &tbl;
        }

        double combine_x1x2_mtable::getx(const double x1, const double x2) const
        {
            auto y = t->getValue(x1);
            for (size_t i = y.size() + 1; i <= 4; i++)
            {
                y.push_back(0.0);
            }
            double y2 = dist.getValue(x2, y[0], y[1], y[2], y[3]);
            double added = x1 + y2;
            return added;
        }
    }
}
