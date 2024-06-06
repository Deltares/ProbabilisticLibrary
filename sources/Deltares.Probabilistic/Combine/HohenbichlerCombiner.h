#pragma once
#include <vector>
#include <memory>
#include "combiner.h"
#include "alphaBeta.h"

namespace Deltares
{
    namespace Reliability
    {
        class HohenbichlerCombiner : public Combiner
        {
        public:
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr) override;
        private:
            /**
             * \brief Combines design points with equal stochasts in alpha values (so designPoints[i]->Alpha[k]->Stochast == designPoints[j]->Alpha[k]->Stochast for all i,j,k)
             * \param designPoints Design points to be combined
             * \param rho Self correlation values for all k
             * \param cmbType parallel or series combination type
             * \return Combined design point
             */
            std::shared_ptr<DesignPoint> CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints, const std::vector<double>& rho, const combineAndOr cmbType);
            alphaBeta copyAlphaBeta(const std::shared_ptr<DesignPoint>& dp);
        };
    };
}
