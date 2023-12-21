#pragma once
#include <vector>
#include <cstddef>
#include "../utils/xyPair.h"

namespace Deltares {
    namespace ProbLibCore {

        class FuncWrapper
        {
            // abstract class to be used in root_finder
        public:
            virtual double GetValue(const double x) = 0;
            virtual double GetValue(const double x, const int threadId) { return GetValue(x); };
        };

        class root_finder
        {
            // find the root of the function f:
        public:
            root_finder(FuncWrapper& f) : func(f) {};
            root_finder(FuncWrapper& f, const bool inBetween) : func(f), mustBeInbetween(inBetween) {};
            root_finder(FuncWrapper& f, const size_t mxi) : func(f), maxIter(mxi) {};
            root_finder(FuncWrapper& f, const double e, const size_t mxi);
            root_finder(FuncWrapper& f, const double e, const size_t mxi, const bool inBetween);

            //  returns pair {success, x} where f(x) == 0.
            //  most implementations assumes at most one root in [x0 ... x1]
            //  (linear_loop is an exception on that)
            //  some implementations will find a root even if it is outside the range x0 ... x1.
            //  see flag mustBeInbetween
            virtual std::pair<bool, double> FindRoot(const double x0, const double x1);

            virtual void getStats(double& y, size_t& steps) const;

        protected:
            // nextXvalue gives the next x value to be used u in the iteration loop
            // this function gives the difference in behaviour (convergence speed).
            virtual double nextXvalue(const xyPair p0, const xyPair p1) const = 0;

            // func is intentionally not const as it may do some administration (see sphere_search)
            FuncWrapper& func;

            const double epsX = 1e-2;
            const double epsY = 1e-2;
            const size_t maxIter = 99;

            std::vector<xyPair> state;

            // flag that the method assumes to find the root x in the range x0 ... x1.
            const bool mustBeInbetween = false;
            void checkParam(const double e);
        };
    }
}
