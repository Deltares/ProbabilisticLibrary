// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include "AdaptiveImportanceSampling.h"
#include "CobylaReliability.h"
#include "CrudeMonteCarlo.h"
#include "StartPointCalculatorSettings.h"
#include "StochastSettingsSet.h"
#include "DesignPointBuilder.h"
#include "DirectionalSampling.h"
#include "DirectionalSamplingThenFORM.h"
#include "FORM.h"
#include "FORMThenDirectionalSampling.h"
#include "LatinHyperCube.h"
#include "NumericalBisection.h"
#include "NumericalIntegration.h"
#include "DirectionReliabilitySettings.h"
#include "ReliabilityMethod.h"
#include "SubsetSimulation.h"
#include "SubsetSimulationSettings.h"
#include "../Model/RandomSettings.h"
#include "../Model/ModelProjectSettings.h"
#include "../Model/GradientSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        enum ReliabilityResultType {ResultDesignPoint, ResultFragilityCurve};

        enum ReliabilityMethodType {ReliabilityFORM, ReliabilityNumericalIntegration, ReliabilityCrudeMonteCarlo,
            ReliabilityImportanceSampling, ReliabilityAdaptiveImportanceSampling, ReliabilityDirectionalSampling,
            ReliabilityNumericalBisection, ReliabilityLatinHyperCube, ReliabilityCobyla,
            ReliabilitySubsetSimulation, ReliabilityFORMthenDirectionalSampling, ReliabilityDirectionalSamplingThenFORM};

        /**
         * \brief General settings applicable to all mechanisms
         * \remarks Should be split up according to SonarCube, but logically belongs together
         */
        class Settings : public ModelProjectSettings
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
             * \brief Defines the way new samples are generated
             */
            SampleMethodType SampleMethod = SampleMethodType::MarkovChain;

            /**
             * \brief The minimum samples to be examined
             */
            int MinimumSamples = 1000;

            /**
             * \brief The maximum samples to be examined
             */
            int MaximumSamples = 10000;

            /**
             * \brief The maximum samples to be examined
             */
            int MaximumSamplesNoResult = 10000;

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
             * \brief Minimum number of iterations in numerical bisection
             */
            int MinimumIterations = 5;

            /**
             * \brief Maximum number of guessed design points in one FORM loop
             */
            int MaximumIterations = 50;

            /**
             * \brief Default number of intervals in numerical integration
             */
            int Intervals = 200;

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
             * \brief Settings for calculating one direction
             */
            std::shared_ptr<DirectionReliabilitySettings> DirectionSettings = std::make_shared<DirectionReliabilitySettings>();

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
            bool isValid() const;

            static std::string getReliabilityResultTypeString(ReliabilityResultType method);
            static ReliabilityResultType getReliabilityResultType(std::string method);
            static std::string getReliabilityMethodTypeString(ReliabilityMethodType method);
            static ReliabilityMethodType getReliabilityMethodType(std::string method);
        private:
            std::shared_ptr<FORM> GetFORMMethod() const;
            std::shared_ptr<NumericalIntegration> GetNumericalIntegrationMethod() const;
            std::shared_ptr<CrudeMonteCarlo> GetCrudeMonteCarloMethod() const;
            std::shared_ptr<ImportanceSampling> GetImportanceSamplingMethod() const;
            std::shared_ptr<AdaptiveImportanceSampling> GetAdaptiveImportanceSamplingMethod() const;
            std::shared_ptr<DirectionalSampling> GetDirectionalSamplingMethod() const;
            std::shared_ptr<NumericalBisection> GetNumericalBisectionMethod() const;
            std::shared_ptr<LatinHyperCube> GetLatinHypercubeMethod() const;
            std::shared_ptr<SubsetSimulation> GetSubsetSimulationMethod() const;
            std::shared_ptr<CobylaReliability> GetCobylaReliabilityMethod() const;
            std::shared_ptr<FORMThenDirectionalSampling> GetFormThenDsReliabilityMethod() const;
            std::shared_ptr<DirectionalSamplingThenFORM> GetDsThenFormReliabilityMethod() const;

        };
    }
}

