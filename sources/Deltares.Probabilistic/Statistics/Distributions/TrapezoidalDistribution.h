#pragma once
#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class TrapezoidalDistribution : public Distribution
        {
        public:
            void initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values) override;
            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
            double getMean(std::shared_ptr<StochastProperties> stochast) override;
            double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
            bool maintainMeanAndDeviation(std::shared_ptr<StochastProperties> stochast) override { return stochast->Minimum == 0 && stochast->Maximum == 0; }
            void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            void setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType) override;
            bool canFit() override { return true; }
            void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values) override;
            bool isValid(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<DistributionPropertyType> getParameters() override { return { Minimum, Maximum, Shift, ShiftB }; }
        private:
            double getExponentDifference(double x, double y, int exp);
        };
    }
}

