#pragma once
#include <utility>
#include <vector>
#include "CombineType.h"
#include "../Reliability/DesignPoint.h"
#include "../Statistics/SelfCorrelationMatrix.h"

namespace Deltares {
    namespace Reliability {

        class Hohenbichler2
        {
        public:
            std::shared_ptr<DesignPoint> AlphaHohenbichler(const std::shared_ptr<DesignPoint>& designPoint1, const std::shared_ptr<DesignPoint>& designPoint2,
                const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts,
                const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelation, const combineAndOr system);
        private:
            double BetaHohenbichler(double dp1, double dp2, double rho, combineAndOr system);
            double HohenbichlerNumInt(double dp1, double dp2, double rho, combineAndOr system);
            std::vector<double> LinearSpaced(int length, double start, double stop);
            const double rhoLimitHohenbichler = 0.98;  // Limit value for the correlation coefficient
        };
    }
}
