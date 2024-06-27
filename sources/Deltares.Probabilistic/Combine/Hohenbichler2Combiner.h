#pragma once
#include <vector>
#include <memory>
#include "combiner.h"

namespace Deltares
{
    namespace Reliability
    {
        class Hohenbichler2Combiner : public Combiner
        {
        public:
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr) override;
        };
    };
}
