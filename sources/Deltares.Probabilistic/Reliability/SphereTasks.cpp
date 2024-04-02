#include "SphereTasks.h"
#include <cmath>
#include "../Math/basic_math.h"

namespace Deltares {
    namespace Reliability {

        std::vector<vector1D> sphereTasks::examineSurfaceForTasks(const vector1D& uRay)
        {
            uTasks.clear();
            auto uRayIpoint = std::vector<int>();
            for (size_t i = 0; i < uRay.size(); i++)
            {
                if (uRay(i) != 0.0)
                {
                    uRayIpoint.push_back(i);
                }
            }
            examineSurfaceForTasks(0, uRay, uRayIpoint);
            return uTasks;
        }

        // Method for the search of all possible u
        void sphereTasks::examineSurfaceForTasks(const size_t idx, const vector1D& uRay, const std::vector<int> uRayIpoint)
        {
            // idx     : index in u array
            // uRay    : direction in which u search takes places

            const size_t nrActiveStochasts = uRayIpoint.size();
            const size_t nrStochasts = uRay.size();
            if (idx + 1 < nrActiveStochasts)
            {
                size_t jMax = (uRay(uRayIpoint[idx]) == 0.0 ? 1 : maxSteps);
                auto u = vector1D(nrStochasts);

                double dangle = M_PI / 2.0 / (double)(maxSteps - 1);
                for (size_t j = 1; j <= jMax; j++)
                {
                    double angle = dangle * double(j - 1);
                    for (size_t k = 0; k <= idx; k++)
                    {
                        u(uRayIpoint[k]) = uRay(uRayIpoint[k]) * sin(angle);
                    }
                    u(uRayIpoint[idx + 1]) = uRay(uRayIpoint[idx + 1]) * cos(angle);
                    for (size_t k = idx + 2; k < nrActiveStochasts; k++)
                    {
                        u(uRayIpoint[k]) = uRay(uRayIpoint[k]);
                    }

                    examineSurfaceForTasks(idx + 1, u, uRayIpoint);
                }
            }
            else
            {
                uTasks.push_back(uRay);
            }
        }
    }
}
