#pragma once
#include <vector>
#include <memory>

#include "CombinedDesignPointModel.h"
#include "combiner.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/DesignPoint.h"
#include "../Reliability/DirectionalSamplingSettings.h"
#include "../Model/ProgressIndicator.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Combines design points with directional sampling algorithm
         */
        class DirectionalSamplingCombiner : public Combiner
        {
        public:
            /**
             * \brief Combines a number of design points
             * \param combineMethodType Series (or-combination) or Parallel (and-combination) combination type
             * \param designPoints Design points to be combined
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \param progress Progress indicator (optional)
             * \return Design point resembling the combined reliability and alpha values
             */
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr) override;
        private:
            void fillSettings(std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<DirectionalSamplingSettings> settings);
        };
    }
}

