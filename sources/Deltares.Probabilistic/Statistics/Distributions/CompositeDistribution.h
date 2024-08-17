#pragma once

#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class CompositeDistribution : public Distribution
        {
        public:
            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
            bool isValid(std::shared_ptr<StochastProperties> stochast) override;
            double getMean(std::shared_ptr<StochastProperties> stochast) override;
            double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<DistributionPropertyType> getParameters() override { return { }; }
        private:
            const double delta = 0.0000001;
        };
    }
}

