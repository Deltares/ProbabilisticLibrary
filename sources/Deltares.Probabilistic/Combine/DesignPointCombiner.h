#pragma once

#include "../Utils/probLibException.h"
#include "../Reliability/DesignPoint.h"
#include "combiner.h"
#include "DirectionalSamplingCombiner.h"
#include "HohenbichlerCombiner.h"
#include "ImportanceSamplingCombiner.h"

namespace Deltares
{
    namespace Reliability
    {
        enum CombinerType {Hohenbichler, ImportanceSampling, DirectionalSampling};

        /**
         * \brief Combines design points
         */
        class DesignPointCombiner
        {
        public:
            DesignPointCombiner() {}
            DesignPointCombiner(CombinerType combinerType)
            {
                this->combinerType = combinerType;
            }

            /**
             * \brief Specifies the combiner algorithm
             */
            CombinerType combinerType = CombinerType::ImportanceSampling;

            /**
             * \brief Combines a number of design points
             * \param combineMethodType Identifies series (or) or parallel (and) combination
             * \param designPoints Design points to be combined
             * \param correlationMatrix Correlation matrix (optional)
             * \param progress Progress indicator (optional)
             * \return Combined design point
             */
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = nullptr, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr)
            {
                const std::shared_ptr<Combiner> combiner = getCombiner();
                return combiner->combineDesignPoints(combineMethodType, designPoints, correlationMatrix, selfCorrelationMatrix, progress);
            }

        private:
            std::shared_ptr<Combiner> getCombiner()
            {
                switch (combinerType)
                {
                case CombinerType::ImportanceSampling: return std::make_shared<ImportanceSamplingCombiner>();
                case CombinerType::Hohenbichler: return std::make_shared<HohenbichlerCombiner>();
                case CombinerType::DirectionalSampling: return std::make_shared<DirectionalSamplingCombiner>();
                default: throw probLibException("Combiner type");
                }
            }
        };
    }
}

