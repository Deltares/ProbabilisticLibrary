#pragma once
#include <vector>
#include <memory>
#include "../Reliability/DesignPoint.h"

namespace Deltares
{
    namespace Reliability
    {
        enum class combineAndOr
        {
            combAnd,
            combOr,
        };

        class combiner
        {
        public:
            virtual std::shared_ptr<DesignPoint> CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints,
                const std::vector<double> & rho, const combineAndOr cmbType) = 0;
        };
    };
}
