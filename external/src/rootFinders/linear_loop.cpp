#include "linear_loop.h"
#include <cstddef>
#include <cmath>
#include <float.h>
#include <vector>
#include "../utils/basic_math.h"

namespace Deltares {
    namespace ProbLibCore {

        linear_loop::linear_loop(FuncWrapper& f) : root_finder(f, (size_t)40), numThreads(1) {}
        linear_loop::linear_loop(FuncWrapper& f, const double eps, const size_t mxi, const ll_final_step ll, const int nTreads) :
            root_finder(f, eps, mxi), final(ll), numThreads(nTreads) {}

        double linear_loop::GetParValues(std::vector<double>& fys, const double x0, const double dx, const int i)
        {
            if (numThreads <= 1)
            {
                fy = func.GetValue(x0 + dx * (double)i);
            }
            else
            {
                int n = mod(i, numThreads);
                if (n == 0)
                {
#pragma omp parallel for num_threads(numThreads)
                    for (int k = 0; k < numThreads; k++)
                    {
                        double xk = x0 + dx * (double)(i + k);
                        fys[k] = func.GetValue(xk, k);
                    }
                }
                fy = fys[n];
            }
            return fy;
        }

        std::pair<bool, double> linear_loop::FindRoot(const double x0, const double x1)
        {
            std::vector<double> fys;
            if (numThreads > 1) { fys = std::vector<double>(numThreads); }

            double dx = (x1 - x0) / (double)maxIter;
            double y0 = GetParValues(fys, x0, dx, 0);
            fy = y0;
            ii = 1;
            if (final == ll_final_step::xminzHR)
            {
                if (y0 < 0.0)
                {
                    return { true, x0 };
                }
            }
            else if (fabs(y0) < epsY)
            {
                return { true, x0 };
            }
            double x = -999.0;
            bool success = false;
            double zmin = fabs(fy);
            double xmin = x0;
            double fyPrev = fy;

            for (int i = 1; i <= (int)maxIter; i++)
            {
                x = x0 + dx * (double)i;
                fy = GetParValues(fys, x0, dx, i);
                ii++;
                if (fabs(fy) <= zmin)
                {
                    xmin = x;
                    zmin = fabs(fy);
                }
                if (final == ll_final_step::xminzHR)
                {
                    if (fy < 0.0)
                    {
                        success = true;
                        break;
                    }
                }
                else if (fabs(fy) < epsY || y0 * fy <= 0.0)
                {
                    success = true;
                    break;
                }
                fyPrev = fy;
            }

            switch (final)
            {
            case ll_final_step::lastx:
                return { success, x };
            case ll_final_step::xminz:
            case ll_final_step::xminzHR:
                return { success, xmin };
            default:
                double q = (fabs(fy) * dx / fabs(fyPrev)) / (1.0 + fabs(fy / fyPrev));
                return { success, x - q };
            }
        }

        void linear_loop::getStats(double& y, size_t& steps) const
        {
            y = fy;
            steps = ii;
        }
    }
}
