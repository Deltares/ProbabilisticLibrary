#pragma once
#include "dsSettings.h"
#include "../utils/vector1D.h"
#include "../rootFinders/root_finder.h"

namespace Deltares {
    namespace ProbLibCore {

        class dsRootFinder
        {
        public:
            dsRootFinder(const DSrootFinderSettings s) : settings(s) {};
            void init(const double z0);
            virtual std::pair<bool, double> FindRoot(FuncWrapper& f, const double rA, const double rB);
        protected:
            double z0 = -999.0;
            const DSrootFinderSettings settings;
        };
    }
}
