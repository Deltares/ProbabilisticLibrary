#pragma once
#include <vector>
#include <memory>
#include "../Reliability/DesignPoint.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Model/ProgressIndicator.h"

namespace Deltares
{
    namespace Reliability
    {
        enum class combineAndOr
        {
            combAnd,
            combOr,
        };

        class Combiner
        {
        public:
            virtual std::shared_ptr<DesignPoint> CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints, const std::vector<double> & rho, const combineAndOr cmbType) = 0;
            virtual std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, std::shared_ptr<ProgressIndicator> progress) = 0;
        };
    };
}
