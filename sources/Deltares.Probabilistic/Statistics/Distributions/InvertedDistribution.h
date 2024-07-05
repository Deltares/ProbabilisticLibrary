#pragma once
#include <memory>

#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class InvertedDistribution : public Distribution
        {
        public:

            InvertedDistribution(std::shared_ptr<Distribution> innerDistribution)
            {
                this->innerDistribution = innerDistribution;
            }

            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
            bool canTruncate() override { return this->innerDistribution->canTruncate(); }
            bool canInvert() override { return true; }
            bool canFit() override { return this->innerDistribution->canFit(); }
            double getMean(std::shared_ptr<StochastProperties> stochast) override;
            double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
            void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
            void setShift(std::shared_ptr<StochastProperties> stochast, double shift, bool inverted) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            void setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType) override;
            void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values) override;
            bool isValid(std::shared_ptr<StochastProperties> stochast) override;
            double getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<DistributionPropertyType> getParameters() override;
        private:
            std::shared_ptr<Distribution> innerDistribution = nullptr;
            double getInvertedValue(std::shared_ptr<StochastProperties> stochast, double value);
            std::shared_ptr<StochastProperties> getInvertedStochast(std::shared_ptr<StochastProperties> stochast);
            void copyFromInverted(std::shared_ptr<StochastProperties> target, std::shared_ptr<StochastProperties> source);
        };
    }
}
