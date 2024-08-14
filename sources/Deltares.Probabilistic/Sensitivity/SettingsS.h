#pragma once

#include "SensitivityMethod.h"
#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"
#include "../Model/GradientSettings.h"

namespace Deltares
{
    namespace Sensitivity
    {
        enum SensitivityMethodType {SensitivityFORM, SensitivityNumericalIntegration, SensitivityCrudeMonteCarlo, SensitivityImportanceSampling, SensitivityDirectionalSampling };

        /**
         * \brief General settings applicable to all mechanisms
         */
        class SettingsS
        {
        public:
            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            SensitivityMethodType SensitivityMethod = SensitivityMethodType::SensitivityCrudeMonteCarlo;

            /**
             * \brief The minimum samples to be examined
             */
            int MinimumSamples = 1000;

            /**
             * \brief The maximum samples to be examined
             */
            int MaximumSamples = 10000;

            /**
             * \brief The minimum directions to be examined
             */
            int MinimumDirections = 1000;

            /**
             * \brief The maximum directions to be examined
             */
            int MaximumDirections = 10000;

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
             * \brief Maximum number of guessed design points in one FORM loop
             */
            int MaximumIterations = 50;

            /**
             * \brief Relaxation factor, which is applied when generating the guessed design point for a new iteration
             */
            double RelaxationFactor = 0.75;

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Settings for generating random values
             */
            std::shared_ptr<Deltares::Models::RandomSettings> RandomSettings = std::make_shared<Deltares::Models::RandomSettings>();

            /**
             * \brief Settings for calculating the gradient at a stochast point
             */
            std::shared_ptr<Models::GradientSettings> GradientSettings = std::make_shared<Models::GradientSettings>();

            /**
             * \brief Settings for individual stochastic variable, such as the start value
             */
            std::shared_ptr<Reliability::StochastSettingsSet> StochastSet = std::make_shared<Reliability::StochastSettingsSet>();

            /**
             * \brief Gets the reliability method and settings based on these settings
             */
            std::shared_ptr<Sensitivity::SensitivityMethod> GetSensitivityMethod();


            /**
             * \brief Indicates whether these settings have valid values
             * \return Indication
             */
            bool isValid();

            static std::string getSensitivityMethodTypeString(SensitivityMethodType method);
            static SensitivityMethodType getSensitivityMethodType(std::string method);
        private:
            const std::shared_ptr<Sensitivity::SensitivityMethod> GetFORMMethod();
            const std::shared_ptr<Sensitivity::SensitivityMethod> GetNumericalIntegrationMethod();
            const std::shared_ptr<Sensitivity::SensitivityMethod> GetCrudeMonteCarloMethod();
            const std::shared_ptr<Sensitivity::SensitivityMethod> GetImportanceSamplingMethod();
            const std::shared_ptr<Sensitivity::SensitivityMethod> GetDirectionalSamplingMethod();
        };
    }
}

