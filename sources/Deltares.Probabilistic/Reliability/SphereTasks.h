#pragma once
#include <vector>
#include "../Math/vector1D.h"
#include "../Math/basic_math.h"

namespace Deltares
{
    namespace Reliability
    {
        class sphereTasks
        {
        public:
            sphereTasks() : maxSteps(5), maxAngle(M_PI / 2.0) {};
            sphereTasks(const size_t mxSteps, const bool allQuadrants) : maxSteps(mxSteps), maxAngle(allQuadrants ? 2.0 * M_PI : M_PI / 2.0) {};
            std::vector<Numeric::vector1D> examineSurfaceForTasks(const Numeric::vector1D& uRay);
        private:
            void examineSurfaceForTasks(const size_t idx, const Numeric::vector1D& uRay, const std::vector<int> uRayIpoint);
            std::vector<Numeric::vector1D> uTasks;
            const size_t maxSteps;
            const double maxAngle;
        };
    }
}
