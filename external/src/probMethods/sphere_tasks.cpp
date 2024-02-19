#include "sphere_tasks.h"
#include "../utils/basic_math.h"
#include <cmath>

namespace Deltares {
    namespace ProbLibCore {

        std::vector<vector1D> sphereTasks::examineSurfaceForTasks(const vector1D& uRay)
        {
            uTasks.clear();
            examineSurfaceForTasks(0, uRay);
            return uTasks;
        }

        // Method for the search of all possible u
        void sphereTasks::examineSurfaceForTasks(const size_t idx, const vector1D& uRay)
        {
            // idx     : index in u array
            // uRay    : direction in which u search takes places

            const size_t nrStochasts = uRay.size();
            if (idx + 1 < uRay.size())
            {
                size_t jMax = (uRay(idx) == 0.0 ? 1 : maxSteps);
                auto u = vector1D(nrStochasts);

                double dangle = M_PI / 2.0 / (double)(maxSteps - 1);
                for (size_t j = 1; j <= jMax; j++)
                {
                    double angle = dangle * double(j - 1);
                    for (size_t k = 0; k <= idx; k++)
                    {
                        u(k) = uRay(k) * sin(angle);
                    }
                    u(idx + 1) = uRay(idx + 1) * cos(angle);
                    for (size_t k = idx + 2; k < nrStochasts; k++)
                    {
                        u(k) = uRay(k);
                    }

                    examineSurfaceForTasks(idx + 1, u);
                }
            }
            else
            {
                uTasks.push_back(uRay);
            }
        }
    }
}
