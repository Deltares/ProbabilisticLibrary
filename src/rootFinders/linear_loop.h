#pragma once
#include "root_finder.h"

namespace Deltares {
    namespace ProbLibCore {

        enum class ll_final_step
        {
            lastx,
            xminz,
            interp,
            xminzHR,
        };

        class linear_loop : public root_finder
        {
        public:
            linear_loop(FuncWrapper& f);
            linear_loop(FuncWrapper& f, const double eps, const size_t mxi, const ll_final_step ll, const int nTreads);
            virtual std::pair<bool, double> FindRoot(const double x0, const double x1);
            virtual void getStats(double& y, size_t& steps) const;
        protected:
            virtual double nextXvalue(const xyPair p0, const xyPair p1) const { return -1; };
            double GetParValues(std::vector<double>& fys, const double x0, const double dx, const int i);
            ll_final_step final = ll_final_step::lastx;
            size_t ii = 0;
            double fy = -999.0;
            const int numThreads;
        };
    }
}
