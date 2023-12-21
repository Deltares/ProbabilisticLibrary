#include "progress.h"
#include <iostream>

namespace Deltares {
    namespace ProbLibCore {

        bool progress::printProgressAndCancel(int nTimes, double x, double y) const
        {
            if (interval > 0)
            {
                if (nTimes % interval == 0)
                {
                    os << "nTimes, Pf = " << nTimes << ", " << x << std::endl;
                }
            }
            return false;
        }
    }
}
