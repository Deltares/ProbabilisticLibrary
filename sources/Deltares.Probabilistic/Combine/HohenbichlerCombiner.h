#pragma once
#include <vector>
#include <memory>
#include "combiner.h"
#include "alphaBeta.h"

namespace Deltares
{
    namespace Reliability
    {
        class HohenbichlerCombiner : public combiner
        {
        public:
            virtual std::shared_ptr<DesignPoint> CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints,
                const std::vector<double>& rho, const combineAndOr cmbType);
        private:
            alphaBeta copyAlphaBeta(const std::shared_ptr<DesignPoint>& dp);
        };
    };
}
