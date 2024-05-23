#pragma once
#include <vector>
#include <memory>
#include "../Reliability/DesignPoint.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Statistics/SelfCorrelationMatrix.h"
#include "../Model/ProgressIndicator.h"

namespace Deltares
{
    namespace Reliability
    {
        enum class combineAndOr
        {
            combAnd,
            combOr,
        };

        /**
         * \brief Super class for all design point combination algorithms
         */
        class Combiner
        {
        public:
            virtual std::shared_ptr<DesignPoint> CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints, const std::vector<double> & rho, const combineAndOr cmbType) = 0;

            /**
             * \brief Combines a number of design points
             * \param combineMethodType Series (or-combination) or Parallel (and-combination) combination type
             * \param designPoints Design points to be combined
             * \param correlationMatrix Correlation matrix (to be removed?)
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \param progress Progress indicator (optional)
             * \return Design point resembling the combined reliability and alpha values
             */
            virtual std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress) = 0;
        };
    };
}
