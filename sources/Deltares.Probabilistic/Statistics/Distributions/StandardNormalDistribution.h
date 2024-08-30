#pragma once

#include <numbers>

#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class StandardNormalDistribution : public Distribution
        {
        public:
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getMean(std::shared_ptr<StochastProperties> stochast) override;
            double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
        private:
            const double standardNormalMean = 0.0;
            const double standardNormalDeviation = 1.0;
            const double normalFactor = 1.0 / sqrt(2.0 * std::numbers::pi);
        };
    }
}

