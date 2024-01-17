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
            FORMSTART = 1234,
        };

        enum class rndTypes {
            GeorgeMarsaglia = 1,
            MersenneTwister
        };

        enum class StartMethods
        {
            Zero = 1,
            One,
            GivenVector,
            RaySearch,
            SphereSearch,
            RaySearchVector = 8,
            RaySearchVectorScaled
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
            MultiplyInvCholesky
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
            int                seed1 = 2;
            int                seed2 = 1;
            rndTypes           rnd = rndTypes::GeorgeMarsaglia;
            StartMethods       startMethod = StartMethods::Zero;
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
        };

    }
}
