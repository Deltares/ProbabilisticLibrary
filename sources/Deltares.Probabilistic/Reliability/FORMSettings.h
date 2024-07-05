#pragma once

#include "StartPointCalculatorSettings.h"
#include "StochastSettingsSet.h"
#include "../Model/GradientSettings.h"
#include "../Model/RunSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Settings for FORM algorithm
         */
        class FORMSettings
        {
        public:
            /**
             * \brief Relaxation factor, which is applied when generating the guessed design point for a new iteration
             */
            double RelaxationFactor = 0.75;

            /**
             * \brief Maximum number of guessed design points in one FORM loop
             */
            int MaximumIterations = 50;

            /**
             * \brief Maximum number of FORM loops when no convergence is reached
             */
            int RelaxationLoops = 1;

            /**
             * \brief Increase factor of Maximum iterations when a new FORM loop is applied
             */
            int MaxIterationsGrowthFactor = 1;

            /**
             * \brief The FORM loop stops when the next guessed design point is less than this value away from the current guessed design point
             */
            double EpsilonBeta = 0.01;

            /**
             * \brief If true, makes a guess for the design point if the final FORM loop does not converge
             */
            bool FilterAtNonConvergence = false;

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Settings for calculating the gradient at a stochast point
             */
            std::shared_ptr<Models::GradientSettings> GradientSettings = std::make_shared<Models::GradientSettings>();

            /**
             * \brief Settings for calculating the initial guessed design point
             */
            std::shared_ptr<StartPointCalculatorSettings> StartPointSettings = std::make_shared<StartPointCalculatorSettings>();

            /**
             * \brief Settings for individual stochastic variable, such as the start value
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            /**
             * \brief Indicates whether these settings have valid values
             * \return Indication
             */
            bool isValid()
            {
                return RelaxationLoops >= 1 &&
                    RelaxationFactor >= 0.01 &&
                    RelaxationFactor <= 1 &&
                    MaximumIterations >= 1 &&
                    EpsilonBeta >= 0.01 &&
                    GradientSettings->isValid() &&
                    StartPointSettings->isValid() &&
                    RunSettings->isValid();
            }
        };
    }
}

