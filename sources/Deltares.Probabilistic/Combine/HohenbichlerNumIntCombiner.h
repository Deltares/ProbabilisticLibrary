#pragma once
#include <vector>
#include <memory>
#include "combiner.h"

namespace Deltares
{
    namespace Reliability
    {
        class HohenbichlerNumIntCombiner : public Combiner
        {
        public:
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr) override;
        private:
            static void findMaxCorrelatedDesignPoints(std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix,
                const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts, long long& i1max, long long& i2max);
        };
    };
}
