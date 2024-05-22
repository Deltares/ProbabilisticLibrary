#pragma once
#include <vector>
#include <memory>
#include "combiner.h"
#include "alphaBeta.h"

namespace Deltares
{
    namespace Reliability
    {
        class HohenbichlerCombiner : public Combiner
        {
        public:
            std::shared_ptr<DesignPoint> CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints, const std::vector<double>& rho, const combineAndOr cmbType) override;
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr) override { return nullptr; }
        private:
            alphaBeta copyAlphaBeta(const std::shared_ptr<DesignPoint>& dp);
        };
    };
}
