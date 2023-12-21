#include <math.h>
#include "dsRootFinderRobust.h"
#include "../utils/basic_math.h"

namespace Deltares {
    namespace ProbLibCore {

        int dsRootFinderRobust::GetNk()
        {
            //
            // Upper bound for determining z = 0
            //
            int n = (int)(0.5 + maximumLengthU / settings.du1);
            if (n * settings.du1 < maximumLengthU) n++;
            return n;
        }

        std::pair<bool, double> dsRootFinderRobust::FindRoot(FuncWrapper& func, const double rA, const double rB)
        {
            //
            // Determine upper boundary
            //
            double rAbove = -999.0;
            double rBelow = -999.0;
            double z = -999.0;
            double zPrev = fabs(z0);
            double zAbove = z;
            double zBelow = z;
            for (int k = 1; k <= nk; k++)
            {
                double lambda = min(k * settings.du1, maximumLengthU);
                z = func.GetValue(lambda);
                z = (z0 >= 0.0 ? z : -z);
                zAbove = z;
                zBelow = zPrev;
                if (z < 0.0)
                {
                    rAbove = lambda;
                    rBelow = settings.du1 * (double)(k - 1);
                    break;
                }
                zPrev = z;
            }

            //
            // Wrong direction (no failure)?
            //
            if (z >= 0.0)
            {
                double rBeta = maximumLengthU;
                return { false, rBeta };
            }

            double rPrev = 2.0 * maximumLengthU;

            double rBeta = -999.0;
            for (int k = 1; k <= iterations1; k++)
            {
                if (useBisection)
                {
                    rBeta = 0.5 * (rBelow + rAbove);
                }
                else
                {
                    rBeta = rBelow + zBelow / (zBelow - zAbove) * (rAbove - rBelow);
                }
                if (std::abs(rPrev - rBeta) <= settings.epsilonDu)
                {
                    return { true, rBeta };
                }

                z = func.GetValue(rBeta);
                z = (z0 >= 0.0 ? z : -z);

                if (z < 0.0)
                {
                    rAbove = rBeta;
                    zAbove = z;
                }
                else
                {
                    rBelow = rBeta;
                    zBelow = z;
                }

                rPrev = rBeta;
            }
            return { false, rBeta };
        }
    }
}
