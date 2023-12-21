#pragma once
#include "dsRootFinder.h"

namespace Deltares {
    namespace ProbLibCore {

        class dsRootFinderRobust : public dsRootFinder
        {
        public:
            dsRootFinderRobust(const DSrootFinderSettings s) :
                dsRootFinder(s),
                useBisection(settings.iterationMethod == DSiterationMethods::DirSamplingIterMethodRobustBisection),
                nk(GetNk()) {};
            virtual std::pair<bool, double> FindRoot(FuncWrapper& f, const double rA, const double rB);
        private:
            int GetNk();
            const bool useBisection;
            const int nk;
        };
    }
}
