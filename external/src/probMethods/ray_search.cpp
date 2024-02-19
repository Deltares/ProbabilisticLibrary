#include "ray_search.h"
#include "../rootFinders/linear_loop.h"
#include "../rootFinders/Anderson_Bjork.h"

namespace Deltares {
    namespace ProbLibCore {

        rs_func_wrapper::rs_func_wrapper(vector1D& d, const zmodel& m, const stochSettings& s, const int threadId) :
            dir(d), model(m), stoch(s), thread(threadId) {};

        void rs_func_wrapper::updateDir(vector1D& d)
        {
            dir = d;
        }

        double rs_func_wrapper::GetValue(const double x)
        {
            const size_t nrStochasts = stoch.getNrStochasts();
            auto drw = probDraw(nrStochasts, 0.0, stoch);
            if (xx.size() == 0) { xx = vector1D(nrStochasts); }
            for (size_t i = 0; i < nrStochasts; i++)
            {
                xx(i) = dir(i) * x;
            }
            drw.setu(xx);
            auto z = model.zfunc(drw, thread);
            return z;
        }

        double rs_func_wrapper::GetValue(const double x, const int threadId)
        {
            const size_t nrStochasts = stoch.getNrStochasts();
            auto drw = probDraw(nrStochasts, 0.0, stoch);
            auto xv = vector1D(nrStochasts);
            for (size_t i = 0; i < nrStochasts; i++)
            {
                xv(i) = dir(i) * x;
            }
            drw.setu(xv);
            auto z = model.zfunc(drw, threadId);
            return z;
        }

        ray_search::ray_search(const zmodel& m, const stochSettings& s, const int n) : form_init(m, s), numCores(n) {}

        ray_search::ray_search(const zmodel& m, const stochSettings& s, const form_init_settings i)
            : form_init(m, s, i), numCores(1) {}

        vector1D ray_search::searchu(vector1D& dir)
        {
            const double uStart = 0.1;
            const double uStep = 0.3;
            const size_t maxSteps = (size_t)(settings.maxu / uStep);

            auto rsf = rs_func_wrapper(dir, mdl, stoch, 0);
            auto rootfinder = linear_loop(rsf, 1e-4, maxSteps, ll_final_step::xminzHR, numCores);
            auto x = rootfinder.FindRoot(uStart, settings.maxu);
            rootfinder.getStats(zmin, zEvaluations);

            return dir * x.second;
        }

        ray_searchAB::ray_searchAB(const zmodel& m, const stochSettings& s) : form_init(m, s) {}

        ray_searchAB::ray_searchAB(const zmodel& m, const stochSettings& s, const form_init_settings i)
            : form_init(m, s, i) {}

        vector1D ray_searchAB::searchu(vector1D& dir)
        {
            auto rsf = rs_func_wrapper(dir, mdl, stoch, 0);
            auto rootfinder = Anderson_Bjork(rsf);
            auto x = rootfinder.FindRoot(0.0, settings.maxu);
            rootfinder.getStats(zmin, zEvaluations);

            return (x.first ? dir * x.second : vector1D(0));
        }
    }
}
