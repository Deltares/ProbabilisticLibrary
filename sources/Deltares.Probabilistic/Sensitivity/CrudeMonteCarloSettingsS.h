#pragma once

#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"

namespace Deltares
{
    namespace Sensitivity
    {
        class CrudeMonteCarloSettingsS
        {
        public:
            /**
             * \brief The number of samples to be examined
             */
            int MaximumSamples = 10000;

            /**
             * \brief The importance sampling algorithm stops when the calculated variation coefficient is less than this value
             */
            double VariationCoefficient = 0.05;

            /**
             * \brief The probability which is used to check whether the calculation has converged
             */
            double ProbabilityForConvergence = 0.05;

            /**
             * \brief Indicates whether the number of samples should be derived from the variation coefficient at the probability for convergence
             */
            bool DeriveSamplesFromVariationCoefficient = false;

            /**
             * \brief Gets the number of runs which is needed to achieve the variation coefficient at the probability for convergence
             */
            int getRequiredSamples();

            /**
             * \brief Modifies the variation coefficient so that the number of required samples matches a given value
             */
            void setRequiredSamples(int samples);

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
            std::shared_ptr<Reliability::StochastSettingsSet> StochastSet = std::make_shared<Reliability::StochastSettingsSet>();

            /**
             * \brief Indicates whether the settings have valid values
             * \return Indication
             */
            bool isValid()
            {
                return MaximumSamples >= 1 && RunSettings->isValid();
            }
        };
    }
}

