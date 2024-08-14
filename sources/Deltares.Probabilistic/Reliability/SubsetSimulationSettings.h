#pragma once

#include "DesignPointBuilder.h"
#include "StochastSettingsSet.h"
#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        enum SampleMethodType { MarkovChain, AdaptiveConditional };

        /**
         * \brief Settings for the subset simulation algorithm
         */
        class SubsetSimulationSettings
        {
        public:
            /**
             * \brief The minimum samples to be examined
             */
            int MinimumSamples = 1000;

            /**
             * \brief The maximum samples to be examined
             */
            int MaximumSamples = 10000;

            /**
             * \brief The importance sampling algorithm stops when the calculated variation coefficient is less than this value
             */
            double VariationCoefficient = 0.05;

            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

            /**
             * \brief Defines the way new samples are generated
             */
            SampleMethodType SampleMethod = SampleMethodType::MarkovChain;

            /**
             * \brief Standard deviation in the Markov chain
             */
            double MarkovChainDeviation = 1;

            /**
             * \brief Fraction of the samples which will be used in the next iteration
             */
            double SubsetFraction = 0.1;

            /**
             * \brief Settings for generating random values
             */
            std::shared_ptr<Deltares::Models::RandomSettings> randomSettings = std::make_shared<Deltares::Models::RandomSettings>();

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Settings for individual stochastic variables, such as the start value
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            /**
             * \brief Indicates whether the settings have valid values
             * \return Indication
             */
            bool isValid()
            {
                return MinimumSamples >= 1 &&
                    MaximumSamples >= MinimumSamples &&
                    RunSettings->isValid();
            }

            static std::string getSampleMethodString(SampleMethodType method);
            static SampleMethodType getSampleMethod(std::string method);
        };
    }
}

