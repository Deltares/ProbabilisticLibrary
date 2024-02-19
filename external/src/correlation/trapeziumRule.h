#pragma once
#include <vector>
#include "../utils/xyPair.h"

namespace Deltares {
    namespace ProbLibCore {

        class trapeziumRule
        {
        public:
            static double calc(const std::vector<xyPair>& xy);
        };
    }
}
