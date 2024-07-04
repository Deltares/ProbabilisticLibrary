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
            static double BetaHohenbichler(double dp1, double dp2, const double rho, const combineAndOr system);
            static double HohenbichlerNumInt(const double dp1, const double dp2, const double rho, const combineAndOr system);
            static DesignPoint GetRealization(const double beta, const std::vector<std::shared_ptr<Models::StochastPointAlpha>>& alpha);
            static DesignPoint GetRealization(const double beta, const std::vector<std::shared_ptr<Models::StochastPointAlpha>>& alpha, const std::vector<double>& values);
            static void copyParameter(const std::shared_ptr<Statistics::Stochast>& stochast, const std::shared_ptr<DesignPoint>& designPoint, std::vector<std::shared_ptr<StochastPointAlpha>>& parameters);
        };
    }
}
