#pragma once
#include <vector>
#include "../Math/vector1D.h"

namespace Deltares {
    namespace Reliability {

        class sphereTasks
        {
        public:
            sphereTasks() : maxSteps(5) {};
            sphereTasks(const size_t mxSteps) : maxSteps(mxSteps) {};
            std::vector<vector1D> examineSurfaceForTasks(const vector1D& uRay);
        private:
            void examineSurfaceForTasks(const size_t idx, const vector1D& uRay);
            std::vector<vector1D> uTasks;
            const size_t maxSteps;
        };
    }
}
