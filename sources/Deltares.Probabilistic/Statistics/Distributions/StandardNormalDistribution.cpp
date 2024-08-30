#include "StandardNormalDistribution.h"

#include "../StochastProperties.h"
#include "../StandardNormal.h"

namespace Deltares
{
    namespace Statistics
    {
        bool StandardNormalDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return true;
        }

        double StandardNormalDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return standardNormalMean;
        }

        double StandardNormalDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return standardNormalDeviation;
        }

        double StandardNormalDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            return u;
        }

        double StandardNormalDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            return x;
        }

        double StandardNormalDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double distance = -x * x / 2;
            return normalFactor * exp(distance);
        }

        double StandardNormalDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            return StandardNormal::getPFromU(x);
        }
    }
}




