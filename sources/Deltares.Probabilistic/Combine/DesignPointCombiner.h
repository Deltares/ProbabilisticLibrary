#pragma once

#include "../Utils/probLibException.h"
#include "../Reliability/DesignPoint.h"
#include "combiner.h"
#include "DirectionalSamplingCombiner.h"
#include "HohenbichlerNumIntCombiner.h"
#include "ImportanceSamplingCombiner.h"
#include "../Math/Randomizers/RandomValueGenerator.h"

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
            DesignPointCombiner(CombinerType combinerType, Numeric::RandomValueGeneratorType randomGenerator)
            {
                this->combinerType = combinerType;
                this->generator = randomGenerator;
            }

            /**
             * \brief Specifies the combiner algorithm
             */
            CombinerType combinerType = CombinerType::ImportanceSampling;

            /**
             * \brief Combines a number of design points
             * \param combineMethodType Identifies series (or) or parallel (and) combination
             * \param designPoints Design points to be combined
             * \param selfCorrelationMatrix Defines auto correlations (optional)
             * \param progress Progress indicator (optional)
             * \return Combined design point
             */
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr)
            {
                const std::shared_ptr<Combiner> combiner = getCombiner();
                return combiner->combineDesignPoints(combineMethodType, designPoints, selfCorrelationMatrix, progress);
            }

            static std::string getCombineTypeString(combineAndOr type);
            static combineAndOr getCombineType(std::string method);
            static std::string getCombinerMethodString(CombinerType type);
            static CombinerType getCombinerMethod(std::string method);

        private:
            std::shared_ptr<Combiner> getCombiner()
            {
                switch (combinerType)
                {
                case CombinerType::ImportanceSampling:
                {
                    auto impSamplingCombiner = std::make_shared<ImportanceSamplingCombiner>();
                    impSamplingCombiner->randomGeneratorType = generator;
                    return impSamplingCombiner;
                }
                case CombinerType::Hohenbichler:
                    return std::make_shared<HohenbichlerNumIntCombiner>();
                case CombinerType::DirectionalSampling:
                {
                    auto directionalSamplingCombiner = std::make_shared<DirectionalSamplingCombiner>();
                    directionalSamplingCombiner->randomGeneratorType = generator;
                    return directionalSamplingCombiner;
                }
                default: throw probLibException("Combiner type");
                }
            }
            Numeric::RandomValueGeneratorType generator = Numeric::ModifiedKnuthSubtractive;
        };
    }
}

