#pragma once
#include <string>
#include <memory>
#include "zmodel.h"
#include "../probFuncs/stochSettings.h"
#include "../probMethods/progress.h"
#include "lsfResult.h"
#include "dsSettings.h"
#include "parseMethod.h"
#include "dsRootFinderRobust.h"
#include "fillStartVector.h"
#include "../rnd/rndDefs.h"

namespace Deltares {
    namespace ProbLibCore {

        const size_t maxActiveStochast = 32;

        enum class runType
        {
            OMP,
            fork,
            sequential,
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

        class parseAndRunMethod
        {
        public:
            parseAndRunMethod(const basicSettings& bs, const progress& pg) : settings(bs), pc(pg) {};
            lsfResult parseAndRun(const ProbMethod method, const zmodel& z, const stochSettings& s, const size_t numCores = 1) const;
            std::vector<lsfResult> parseAndRunTree(const ProbMethod method, const std::vector<std::unique_ptr<zmodel>>& z,
                const stochSettings& s, const std::string& tree, const runType type, std::ostream& f) const;
            lsfResult parseRunAndPrint(const ProbMethod method, const zmodel& z, const stochSettings& s, std::ostream& f, const size_t numCores = 1) const;
            static void stop();
        private:
            dsRootFinder* getRootFinder(const basicSettings& settings) const;
            basicSettings settings;
            const progress& pc;
        };
    }
}
