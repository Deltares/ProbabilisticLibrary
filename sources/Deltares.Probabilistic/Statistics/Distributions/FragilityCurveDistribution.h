#pragma once

#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class FragilityCurveDistribution : public Distribution
        {
            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override { return true; }
            bool maintainMeanAndDeviation(std::shared_ptr<StochastProperties> stochast) override { return false; }
            void initializeForRun(std::shared_ptr<StochastProperties> stochast) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool canFit() override { return false; }
            bool isValid(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
        private:
            const double precision = 0.00001;

            bool isHorizontal(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues);
            bool startsHorizontal(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues);
            bool isAscending(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues);
            bool isDescending(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues);
            bool endsHorizontal(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues);
        };
    }
}

