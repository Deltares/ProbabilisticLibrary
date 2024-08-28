#pragma once

#include "StartPointCalculatorSettings.h"
#include "StochastSettingsSet.h"
#include "DesignPointBuilder.h"
#include "ReliabilityMethod.h"
#include "SubsetSimulationSettings.h"
#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"
#include "../Model/GradientSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        enum ReliabilityResultType {ResultDesignPoint, ResultFragilityCurve};

        enum ReliabilityMethodType {ReliabilityFORM, ReliabilityNumericalIntegration, ReliabilityCrudeMonteCarlo, ReliabilityImportanceSampling, ReliabilityAdaptiveImportanceSampling, ReliabilityDirectionalSampling, ReliabilitySubsetSimulation };

        /**
         * \brief General settings applicable to all mechanisms
         */
        class Settings
        {
        public:
            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            ReliabilityResultType ReliabilityResult = ReliabilityResultType::ResultDesignPoint;

            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            ReliabilityMethodType ReliabilityMethod = ReliabilityMethodType::ReliabilityFORM;

            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

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
             * \brief The minimum directions to be examined
             */
            int MinimumVarianceLoops = 1;

            /**
             * \brief The maximum directions to be examined
             */
            int MaximumVarianceLoops = 5;

            /**
             * \brief The importance sampling algorithm stops when the calculated variation coefficient is less than this value
             */
            double VariationCoefficient = 0.05;

            /**
             * \brief The calls to importance sampling will be stopped until the fraction of failed samples (Z < 0) is between this value and 1 - this value
             */
            double FractionFailed = 0.1;

            /**
             * \brief Maximum number of guessed design points in one FORM loop
             */
            int MaximumIterations = 50;

            /**
             * \brief Relaxation factor, which is applied when generating the guessed design point for a new iteration
             */
            double RelaxationFactor = 0.75;

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
             * \brief Method type how to generate new samples for subset simulation
             */
            SampleMethodType sampleMethod = SampleMethodType::MarkovChain;

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
             * \brief Settings for calculating the initial guessed design point
             */
            std::shared_ptr<StartPointCalculatorSettings> StartPointSettings = std::make_shared<StartPointCalculatorSettings>();

            /**
             * \brief Settings for individual stochastic variable, such as the start value
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            /**
             * \brief Gets the reliability method and settings based on these settings
             */
            std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod();


            /**
             * \brief Indicates whether these settings have valid values
             * \return Indication
             */
            bool isValid();

            static std::string getReliabilityResultTypeString(ReliabilityResultType method);
            static ReliabilityResultType getReliabilityResultType(std::string method);
            static std::string getReliabilityMethodTypeString(ReliabilityMethodType method);
            static ReliabilityMethodType getReliabilityMethodType(std::string method);
        private:
            const std::shared_ptr<Reliability::ReliabilityMethod> GetFORMMethod();
            const std::shared_ptr<Reliability::ReliabilityMethod> GetNumericalIntegrationMethod();
            const std::shared_ptr<Reliability::ReliabilityMethod> GetCrudeMonteCarloMethod();
            const std::shared_ptr<Reliability::ReliabilityMethod> GetImportanceSamplingMethod();
            const std::shared_ptr<Reliability::ReliabilityMethod> GetAdaptiveImportanceSamplingMethod();
            const std::shared_ptr<Reliability::ReliabilityMethod> GetDirectionalSamplingMethod();
            const std::shared_ptr<Reliability::ReliabilityMethod> GetSubsetSimulationMethod();
        };
    }
}

