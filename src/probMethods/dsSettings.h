#pragma once
#include "dpoptions.h"

namespace Deltares {
    namespace ProbLibCore {

        const double maximumLengthU = 20.0;
        const double almostZero = 1e-200;
        const double maxStepSize = 3.0;
        const double slopeCrit = 4.0;
        const int iterations1 = 20;

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

        struct DSrootFinderSettings
        {
            double epsilonDu = 0.001;
            double du1 = 3.0;
            DSiterationMethods iterationMethod = DSiterationMethods::DirSamplingIterMethodFast;
        };

        struct DSsettings
        {
            // samples in DS is the number of directions; each direction use a few z-evaluations
            // to find z=0 along that direction.
            int maxSamples = 99000;
            int minSamples = 10000;
            double varCoeffNoFailure = 0.005;
            double varCoeffFailure = 0.005;
            int chunkSize = 200;  // block size in OpenMP part
            DPoptions designPointOption = DPoptions::None;
        };
    }
}
