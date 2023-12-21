#include "waarts1.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        double waarts1::zfunc(const probDraw& d) const
        {
            return d.getx(1) - d.getx(0);
        }

        double simple::zfunc(const probDraw& d) const
        {
            const double x1 = d.getx(0);
            const double x2 = d.getx(1);
            const double valueB = d.getx(2);
            return valueB - x1 - x2;
        }
    }
}
