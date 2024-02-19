#pragma once
#include <ostream>
#include <fstream>
#include <string>
#include "../utils/vector1D.h"
#include "../probFuncs/alphaBeta.h"
#include "../probFuncs/stochSettings.h"
#include "../probMethods/zmodel.h"
#include "dpoptions.h"

namespace Deltares {
    namespace ProbLibCore {

        enum class ConvergenceStatus
        {
            success,
            noConvergence,
            abortedByUser,
            notStartedYet,
        };

        class lsfResult
        {
        public:
            lsfResult() {};
            lsfResult(const size_t n);
            lsfResult(const size_t n, const DPoptions d);
            ~lsfResult();
            void printResult(std::ostream& o, const stochSettings& s);
            void dumpResult(std::ofstream& o);
            void readDumpFile(std::fstream& o);
            alphaBeta result;
            vector1D x;
            vector1D u;
            vector1D alpha_u;
            ConvergenceStatus convergence = ConvergenceStatus::notStartedYet;
            int stepsNeeded = -999;
            int samplesNeeded = -999;
            int samplesFailure = -999;
            std::string method;
            void calcXinDesignPoint(const stochSettings& s);
            void calcXinDesignPoint(const zmodel& m, const stochSettings& s);
        private:
            void calcXfromU(const stochSettings& s);
            DPoptions designPointOption = DPoptions::RMin;
            void calcXfromAlpha(const stochSettings& s);
        };
    }
}
