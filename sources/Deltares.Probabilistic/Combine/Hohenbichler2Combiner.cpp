#include "Hohenbichler2Combiner.h"
#include "Hohenbichler2.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> Hohenbichler2Combiner::combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress)
        {
            if (designPoints.empty()) throw probLibException("no design point in combiner");
            if (designPoints.size() == 1) return designPoints[0];

            const std::vector<std::shared_ptr<Statistics::Stochast>> stochasts = getUniqueStochasts(designPoints);

            auto hh = Hohenbichler2();
            auto dp = hh.AlphaHohenbichler(designPoints[0], designPoints[1], stochasts, selfCorrelationMatrix, combineMethodType);
            for (size_t i = 1; i < designPoints.size(); i++)
            {
                dp = hh.AlphaHohenbichler(designPoints[i], dp, stochasts, selfCorrelationMatrix, combineMethodType);
            }

            return dp;
        }

    };
}

