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
#include <string>

namespace Deltares {
    namespace ProbLibCore {

        const size_t maxActiveStochast = 32;

        enum class ProbMethod
        {
            FORM = 1,
            CM = 3,
            DS = 4,
            NI = 5,
            IM = 6,
            AdaptiveIM = 7,
            FDIR = 11,
            DSFIHR,
            CMFORM,
            CMIMFORM,
            FORMDSFIHR,
            DSFI,
            FORMDSFI,
            LatinHyperCube = 18,
            NumericalBisection,
            CobylaReliability,
            SubSetSimulation,
            FORMSTART = 1234,
        };

        enum class StartMethods
        {
            Zero = 1,
            One,
            GivenVector,
            RaySearch,
            SphereSearch,
            RaySearchVector = 8,
            RaySearchVectorScaled,
            Sensitivity = 11
        };

        enum class DPoptions
        {
            None,
            XfromU,
            XCorrelatedFromU,
            RMin,
            RMinZFunc,
            XCorrelatedFromUCompatible,
            RMinZFuncCompatible,
            MultiplyInvCholesky,
            CenterOfGravity,
            CenterOfAngles,
        };

        enum class DSiterationMethods {
            DirSamplingIterMethodRobust = 1,
            DirSamplingIterMethodFast,
            DirSamplingIterMethodFaster,
            DirSamplingIterMethodRobustBisection,
            DirSamplingIterMethodFastBisection,
            DirSamplingIterMethodFasterBisection,
            DirSamplingIterMethodFastAuto = 8,
            DirSamplingIterMethodFasterAuto
        };

        struct corrStruct
        {
            int idx1;
            int idx2;
            double correlation;
        };

        struct numericalBisectionSettings
        {
            int MinimumIterations;
            int MaximumIterations;
            double EpsilonBeta;
        };

        struct latinHypercubeSettings
        {
            int MinimumSamples;
        };

        struct cobylaReliabilitySettings
        {
            double EpsilonBeta;
            int MaximumIterations;
        };

    	struct subSetSimulationReliabilitySettings
        {
            double VariationCoefficient;
            double MarkovChainDeviation;
            int MinimumIterations;
            int MaximumIterations;
            int SampleMethod;
        };

        struct basicSettings
        {
            ProbMethod         methodId;
            double             tolA = 0.001;
            double             tolB = 0.001;
            double             tolC = 0.001;
            int                numThreads;
            int                chunkSize = 200;
            int                minSamples = 10000;
            int                maxSamples = 99000;
            int                seed  = 2;
            int                isRepeatableRandom = 1;
            StartMethods       startMethod = StartMethods::Zero;
            int                maxStepsSphereSearch = 5;
            int                progressInterval = -1;
            double             varianceFactor = 1.5;
            double             epsilonDu = 0.001;
            DPoptions          designPointOptions = DPoptions::RMin;
            double             du = 0.01;
            double             du1 = 3.0;
            double             relaxationFactor = 0.7;
            int                trialLoops = 3;
            DSiterationMethods iterationMethod = DSiterationMethods::DirSamplingIterMethodFast;
            int                numExtraInt = 100;
            int                numExtraInt2 = 0;
            double             numExtraReal1 = -5.0;
            double             numExtraReal2 = 5.0;
            double             startVector[maxActiveStochast];
            double             offsets[maxActiveStochast];
            double             varianceFactors[maxActiveStochast];
            bool               allQuadrants = false;
            bool               saveEvaluations = false;
            numericalBisectionSettings numerical_bisection_settings;
            latinHypercubeSettings latin_hypercube_settings;
            cobylaReliabilitySettings cobyla_reliability_settings;
            subSetSimulationReliabilitySettings sub_set_simulation_reliability_settings;
        };

    }
}
